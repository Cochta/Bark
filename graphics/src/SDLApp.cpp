#include "SDLApp.h"

#ifdef TRACY_ENABLE
#include "Tracy.hpp"
#include "TracyC.h"
#endif 

void SDLApp::SetUp()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
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
		SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
		return;
	}


	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (_renderer == nullptr)
	{
		SDL_Log("SDL_CreateRenderer Error: %s", SDL_GetError());
		return;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

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
			ImGui_ImplSDL2_ProcessEvent(&e);
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
			case SDL_MOUSEBUTTONUP:

			if (e.button.button == SDL_BUTTON_LEFT) {
				_sampleManager.GiveLeftMouseClickToSample();
			}
			else if (e.button.button == SDL_BUTTON_RIGHT) {
				_sampleManager.GiveRightMouseClickToSample();
			}
			break;
			}
		}

		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

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

		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
		SDL_RenderClear(_renderer);

		SDL_GetMouseState(&MousePos.X, &MousePos.Y);
		_sampleManager.GiveMousePositionToSample({ (float)MousePos.X, Metrics::Height - (float)MousePos.Y });
		_sampleManager.UpdateSample();

		DrawAllGraphicsData();

		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(_renderer);

#ifdef TRACY_ENABLE
		FrameMark;
#endif 
	}
}

void SDLApp::DrawCircle(const Math::Vec2F center, const float radius, const int segments, const SDL_Color& col) noexcept
{
#ifdef TRACY_ENABLE
	ZoneScoped;
#endif
	int offset = static_cast<int>(_vertices.size());

	_vertices.reserve(offset + segments - 1);
	_indices.reserve(_indices.size() + (segments - 1) * 3);

	for (int i = 0; i < segments; ++i)
	{
		auto angle = Math::Radian(2.f * Math::Pi * static_cast<float>(i) / static_cast<float>(segments));
		float x = center.X + radius * Math::Cos(angle);
		float y = center.Y + radius * Math::Sin(angle);
		_vertices.push_back({ {x, Metrics::Height - y}, col, {1.0f, 1.0f} });
	}

	for (int i = 0; i < segments - 1; ++i)
	{
		_indices.push_back(offset); 
		_indices.push_back(offset + i);
		_indices.push_back(offset + i + 1);
	}
	_indices.push_back(offset); 
	_indices.push_back(offset + segments - 1);
	_indices.push_back(offset);
}

void SDLApp::DrawRectangle(const Math::Vec2F minBound, const Math::Vec2F maxBound, const SDL_Color& col) noexcept
{
	auto offset = static_cast<int>(_vertices.size());

	_vertices.reserve(offset + 4);
	_indices.reserve(_indices.size() + 6);


	_vertices.push_back({ {minBound.X,Metrics::Height - minBound.Y}, col, {1.0f, 1.0f} });
	_vertices.push_back({ {maxBound.X,Metrics::Height - minBound.Y}, col, {1.0f, 1.0f} });
	_vertices.push_back({ {minBound.X, Metrics::Height - maxBound.Y}, col, {1.0f, 1.0f} });
	_vertices.push_back({ {maxBound.X,Metrics::Height - maxBound.Y}, col, {1.0f, 1.0f} });

	_indices.push_back(offset);                 // Top left vertex
	_indices.push_back(offset + 1);             // Top right vertex
	_indices.push_back(offset + 2);             // Bottom left vertex
	_indices.push_back(offset + 2);             // Bottom left vertex
	_indices.push_back(offset + 1);             // Top right vertex
	_indices.push_back(offset + 3);             // Bottom right vertex
}

void SDLApp::DrawRectangleBorder(const Math::Vec2F minBound, const Math::Vec2F maxBound, const SDL_Color& col) noexcept
{
	DrawRectangle(minBound, { maxBound.X, minBound.Y + 1.0f }, col);

	DrawRectangle(minBound, { minBound.X + 1.0f, maxBound.Y }, col);

	DrawRectangle({ minBound.X, maxBound.Y - 1.0f }, maxBound, col);

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

	_vertices.reserve(offset + numVertices);
	_indices.reserve(_indices.size() + numVertices * 3);

	for (const Math::Vec2F& v : vertices)
	{
		_vertices.push_back({ { v.X,Metrics::Height - v.Y}, col, {1.0f, 1.0f} });
	}

	for (size_t i = 0; i < numVertices - 1; ++i)
	{
		_indices.push_back(offset);
		_indices.push_back(offset + i);
		_indices.push_back(offset + i + 1);
	}

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
