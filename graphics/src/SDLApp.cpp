//
// Created by Coch on 09.10.2023.
//
#include "SDLApp.h"

#ifdef TRACY_ENABLE
#include "Tracy.hpp"
#include "TracyC.h"
#endif // TRACY_ENABLE

void SDLApp::SetUp()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		// Handle SDL initialization error
		SDL_Log("SDL_Init Error: %s", SDL_GetError());
		return;
	}
	_window = SDL_CreateWindow(
		Title.data(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		Width,
		Height,
		SDL_WINDOW_SHOWN);

	if (_window == nullptr)
	{
		// Handle window creation error
		SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
		return;
	}


	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (_renderer == nullptr)
	{
		// Handle renderer creation error
		SDL_Log("SDL_CreateRenderer Error: %s", SDL_GetError());
		return;
	}


	// Initialize ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark;

	ImGui_ImplSDL2_InitForSDLRenderer(_window, _renderer);
	ImGui_ImplSDLRenderer2_Init(_renderer);

	_sampleManager.SetUp();
}

void SDLApp::TearDown() const noexcept
{
	ImGui_ImplSDL2_Shutdown();
	ImGui_ImplSDLRenderer2_Shutdown();

	if (_renderer != nullptr)
	{
		SDL_DestroyRenderer(_renderer);
	}
	if (_window != nullptr)
	{
		SDL_DestroyWindow(_window);
	}
	SDL_Quit();
}

void SDLApp::Run() noexcept
{
	bool quit = false;
	SDL_Event e;

	bool adjustWindow = true;

	while (!quit)
	{

		while (SDL_PollEvent(&e))
		{
			ImGui_ImplSDL2_ProcessEvent(&e); // Process ImGui events
			switch (e.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYUP:
				switch (e.key.keysym.sym)
				{
				case SDLK_LEFT:
					_sampleManager.PreviousSample();
					break;
				case SDLK_RIGHT:
					_sampleManager.NextSample();
					break;
				case SDLK_SPACE:
					_sampleManager.RegenerateSample();
					break;
				}
				break;
			}
		}

		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		static float f = 0.0f;
		static int counter = 0;

		if (adjustWindow)
		{
			ImGui::SetNextWindowSize(ImVec2(Metrics::Width / 3, Metrics::Height / 5));
			adjustWindow = false;
		}


		ImGui::Begin("Sample Manager");

		if (ImGui::BeginCombo("Select a Sample", _sampleManager.GetSampleName(_sampleManager.GetCurrentIndex()).c_str()))
		{
			for (std::size_t index = 0; index < _sampleManager.GetSampleNbr(); index++)
			{
				if (ImGui::Selectable(_sampleManager.GetSampleName(index).c_str(), _sampleManager.GetCurrentIndex() == index))
				{
					_sampleManager.ChangeSample(index);
				}
			}
			ImGui::EndCombo();
		}

		ImGui::Spacing();

		ImGui::TextWrapped(_sampleManager.GetSampleDescription(_sampleManager.GetCurrentIndex()).c_str());

		ImGui::Spacing();

		ImGui::SetCursorPosY(ImGui::GetWindowHeight() - (ImGui::GetFrameHeightWithSpacing()));

		if (ImGui::ArrowButton("PreviousSample", ImGuiDir_Left))
		{
			_sampleManager.PreviousSample();
		}

		ImGui::SameLine();

		if (ImGui::Button("Regenerate"))
		{
			_sampleManager.RegenerateSample();
		}

		ImGui::SameLine();

		if (ImGui::ArrowButton("NextSample", ImGuiDir_Right))
		{
			_sampleManager.NextSample();
		}

		ImGui::End();

		ImGui::Render();

		// Clear the renderer
		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
		SDL_RenderClear(_renderer);

		SDL_GetMouseState(&MousePos.X, &MousePos.Y);
		_sampleManager.GiveMousePositionToSample(static_cast<Math::Vec2F>(MousePos));
		_sampleManager.UpdateSample();

		DrawAllGraphicsData();

		// Present the renderer
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(_renderer);

#ifdef TRACY_ENABLE
		FrameMark;
#endif // TRACY_ENABLE
	}
}

void SDLApp::DrawCircle(const Math::Vec2F center, const float radius, const int segments, const SDL_Color& col) noexcept
{
#ifdef TRACY_ENABLE
	ZoneScoped;
#endif
	int offset = static_cast<int>(_vertices.size());

	// Reserve space for the vertices and indices in advance to avoid reallocation.
	_vertices.reserve(offset + segments - 1);
	_indices.reserve(_indices.size() + (segments - 1) * 3);

	// Calculate vertices for the Circle
	for (int i = 0; i < segments; ++i)
	{
		auto angle = Math::Radian(2.f * Math::Pi * static_cast<float>(i) / static_cast<float>(segments));
		float x = center.X + radius * Math::Cos(angle);
		float y = center.Y + radius * Math::Sin(angle);
		_vertices.push_back({ {x, y}, col, {1.0f, 1.0f} });
	}

	// Calculate indices to create triangles for filling the Circle
	for (int i = 0; i < segments - 1; ++i)
	{
		_indices.push_back(offset); // Center point
		_indices.push_back(offset + i);
		_indices.push_back(offset + i + 1);
	}
	_indices.push_back(offset); // Center point
	_indices.push_back(offset + segments - 1);
	_indices.push_back(offset);  // Connect the last vertex to the center
}

void SDLApp::DrawRectangle(const Math::Vec2F minBound, const Math::Vec2F maxBound, const SDL_Color& col) noexcept
{
	auto offset = static_cast<int>(_vertices.size());
	// Reserve space for the vertices and indices in advance to avoid reallocation.
	_vertices.reserve(offset + 4);
	_indices.reserve(_indices.size() + 6);


	_vertices.push_back({ {minBound.X, minBound.Y}, col, {1.0f, 1.0f} });
	_vertices.push_back({ {maxBound.X, minBound.Y}, col, {1.0f, 1.0f} });
	_vertices.push_back({ {minBound.X, maxBound.Y}, col, {1.0f, 1.0f} });
	_vertices.push_back({ {maxBound.X, maxBound.Y}, col, {1.0f, 1.0f} });

	_indices.push_back(offset);                 // Top left vertex
	_indices.push_back(offset + 1);             // Top right vertex
	_indices.push_back(offset + 2);             // Bottom left vertex
	_indices.push_back(offset + 2);             // Bottom left vertex
	_indices.push_back(offset + 1);             // Top right vertex
	_indices.push_back(offset + 3);             // Bottom right vertex
}

void SDLApp::DrawRectangleBorder(const Math::Vec2F minBound, const Math::Vec2F maxBound, const SDL_Color& col) noexcept
{
	// Draw the top border
	DrawRectangle(minBound, { maxBound.X, minBound.Y + 1.0f }, col);

	// Draw the left border
	DrawRectangle(minBound, { minBound.X + 1.0f, maxBound.Y }, col);

	// Draw the bottom border
	DrawRectangle({ minBound.X, maxBound.Y - 1.0f }, maxBound, col);

	// Draw the right border
	DrawRectangle({ maxBound.X - 1.0f, minBound.Y }, maxBound, col);
}

void SDLApp::DrawPolygon(const std::vector<Math::Vec2F>& vertices, const SDL_Color& col)
{
	if (vertices.size() < 3)
	{
		throw std::invalid_argument("Polygon must have at least 3 vertices");
	}

	size_t offset = _vertices.size();
	size_t numVertices = vertices.size();

	// Reserve space for the vertices and indices in advance to avoid reallocation.
	_vertices.reserve(offset + numVertices);
	_indices.reserve(_indices.size() + numVertices * 3);

	// Add vertices to the _vertices vector.
	for (const Math::Vec2F& v : vertices)
	{
		_vertices.push_back({ {v.X, v.Y}, col, {1.0f, 1.0f} });
	}

	// Add indices to connect the vertices and form triangles.
	for (size_t i = 0; i < numVertices - 1; ++i)
	{
		_indices.push_back(offset);
		_indices.push_back(offset + i);
		_indices.push_back(offset + i + 1);
	}

	// Connect the last vertex to the first vertex to close the polygon.
	_indices.push_back(offset);
	_indices.push_back(offset + numVertices - 1);
	_indices.push_back(offset + 1);
}

void SDLApp::DrawAllGraphicsData() noexcept
{
#ifdef TRACY_ENABLE
	ZoneScoped;
#endif
	_vertices.clear();
	_indices.clear();
	for (auto& bd : _sampleManager.GetSampleData())
	{

		if (bd.Shape.index() == (int)Math::ShapeType::Circle)
		{
			auto& circle = std::get<Math::CircleF>(bd.Shape);
			DrawCircle(circle.Center(), circle.Radius(), 30, {
					static_cast<Uint8>(bd.Color.r),
					static_cast<Uint8>(bd.Color.g),
					static_cast<Uint8>(bd.Color.b),
					static_cast<Uint8>(bd.Color.a)
				});
		}
		else if (bd.Shape.index() == (int)Math::ShapeType::Rectangle)
		{
			auto& rect = std::get<Math::RectangleF>(bd.Shape);
			if (!bd.Filled)
			{
				DrawRectangleBorder(rect.MinBound(), rect.MaxBound(), {
						static_cast<Uint8>(bd.Color.r),
						static_cast<Uint8>(bd.Color.g),
						static_cast<Uint8>(bd.Color.b),
						static_cast<Uint8>(bd.Color.a)
					});
			}
			else
			{
				DrawRectangle(rect.MinBound(), rect.MaxBound(), {
						static_cast<Uint8>(bd.Color.r),
						static_cast<Uint8>(bd.Color.g),
						static_cast<Uint8>(bd.Color.b),
						static_cast<Uint8>(bd.Color.a)
					});
			}
		}
		else if (bd.Shape.index() == (int)Math::ShapeType::Polygon)
		{
			auto& polygon = std::get<Math::PolygonF>(bd.Shape);
			DrawPolygon(polygon.Vertices(), {
					static_cast<Uint8>(bd.Color.r),
					static_cast<Uint8>(bd.Color.g),
					static_cast<Uint8>(bd.Color.b),
					static_cast<Uint8>(bd.Color.a)
				});
		}
	}
#ifdef TRACY_ENABLE
	ZoneNamedN(Render, "Render", true);
#endif
	SDL_RenderGeometry(_renderer, nullptr, _vertices.data(), _vertices.size(), _indices.data(), _indices.size());
}
