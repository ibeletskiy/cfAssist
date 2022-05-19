#include <Python.h>
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include "InputField.h"
#include "CircleButton.h"
#include "Participant.h"
#include "Line.h"

using namespace sf;

double GetAverageRating(Participant& person, const std::string& type_task) {
	std::set<std::string> was;
	int OK = 0, ALL = 0, SM = 0;
	for (Task x : person.ListAllTask()) {
		if (was.find(x.GetTaskName()) != was.end()) {
			continue;
		}
		auto ids = x.GetTags();
		if (ids.find(type_task) != ids.end()) {
			ALL++;
			if (x.GetVerdict()) {
				OK++;
				SM += x.GetRating();
			}
			was.insert(x.GetTaskName());
		}
	}
	if (!OK) return { INF };
	return { SM / (double)OK };
}

double GetStability(Participant& person, const std::string& type_task) {
	std::set<std::string> was;
	int OK = 0, ALL = 0, SM = 0;
	for (Task x : person.ListAllTask()) {
		if (was.find(x.GetTaskName()) != was.end()) {
			continue;
		}
		auto ids = x.GetTags();
		if (ids.find(type_task) != ids.end()) {
			ALL++;
			if (x.GetVerdict()) {
				OK++;
				SM += x.GetRating();
			}
			was.insert(x.GetTaskName());
		}
	}
	if (!ALL) return { INF };
	return { OK / (double)ALL };
}

int GetOK(Participant& person, const std::string& type_task) {
	std::set<std::string> was;
	int OK = 0, ALL = 0, SM = 0;
	for (Task x : person.ListAllTask()) {
		if (was.find(x.GetTaskName()) != was.end()) {
			continue;
		}
		auto ids = x.GetTags();
		if (ids.find(type_task) != ids.end()) {
			ALL++;
			if (x.GetVerdict()) {
				OK++;
				SM += x.GetRating();
			}
			was.insert(x.GetTaskName());
		}
	}
	return OK;
}

int GetALL(Participant& person, const std::string& type_task) {
	std::set<std::string> was;
	int OK = 0, ALL = 0, SM = 0;
	for (Task x : person.ListAllTask()) {
		if (was.find(x.GetTaskName()) != was.end()) {
			continue;
		}
		auto ids = x.GetTags();
		if (ids.find(type_task) != ids.end()) {
			ALL++;
			if (x.GetVerdict()) {
				OK++;
				SM += x.GetRating();
			}
			was.insert(x.GetTaskName());
		}
	}
	return ALL;
}

int GetContestOK(Participant& person, const std::string& type_task) {
	std::set<std::string> was;
	int OK = 0, ALL = 0, SM = 0;
	for (Task x : person.ListAllTask()) {
		if (was.find(x.GetTaskName()) != was.end()) {
			continue;
		}
		auto ids = x.GetTags();
		if (ids.find(type_task) != ids.end()) {
			if (!x.GetSolvingType()) continue;
			ALL++;
			if (x.GetVerdict()) {
				OK++;
				SM += x.GetRating();
			}
			was.insert(x.GetTaskName());
		}
	}
	return OK;
}

Participant makeUser(const std::string& name) {
	std::ofstream os("handle.txt");
	os << name << '\n';
	os.close();
	Py_Initialize();
	PyObject* obj = Py_BuildValue("s", "apiRequestAttempts.pyw");
	FILE* fp = _Py_fopen_obj(obj, "r");
	PyRun_SimpleFile(fp, "apiRequestAttempts.pyw");
	Py_Finalize();
	return Participant("response.txt");
}

std::string getSubject(const std::string& now) {
	RenderWindow window(VideoMode(1450, 1000), "cfAssist", Style::Close | Style::Titlebar);
	std::string ans = "";

	Color back(24, 4, 36, 255);
	Color text = Color::White;

	int n = 12, m = 3;

	std::vector<std::vector<CircleButton>> buttons(n, std::vector<CircleButton>(m));

	std::ifstream in("tags.txt");

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			buttons[i][j].setButtonSize(10, 3);
			buttons[i][j].setButtonColor(back, text, text);
			std::string name;
			std::getline(in, name);
			buttons[i][j].setTitle(name, 22, text);
			buttons[i][j].setButtonPosition(Vector2f(50 + 510 * j, 50 + 80 * i));
			buttons[i][j].setTitlePosition(Vector2f(95 + 510 * j, 48 + 80 * i));
		}
	}

	int indi = -1, indj = -1;

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			if (buttons[i][j].title.getString() == now) {
				buttons[i][j].was_pressed = true;
				indi = i;
				indj = j;
			}
		}
	}
	
	in.close();

	while (window.isOpen()) {
		window.clear(back);
		Event event;
		Vector2i mouse_position = Mouse::getPosition(window);
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed || (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter)) {
				window.close();
			}
			for (int i = 0; i < n; ++i) {
				for (int j = 0; j < m; ++j) {
					if (buttons[i][j].pressed(mouse_position, event)) {
						if (buttons[i][j].was_pressed) {
							buttons[i][j].was_pressed = false;
							indi = -1;
							indj = -1;
						} else {
							if (indi == -1) {
								buttons[i][j].was_pressed = true;
								indi = i;
								indj = j;
							} else {
								buttons[i][j].was_pressed = true;
								buttons[indi][indj].was_pressed = false;
								indi = i;
								indj = j;
							}
						}
					}
				}
			}
		}

		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				buttons[i][j].draw(window);
			}
		}
		window.display();
	}
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			if (buttons[i][j].was_pressed) {
				ans = buttons[i][j].title.getString();
			}
		}
	}
	return ans;
}

void updateSubject(Text& text, int x, Button& edit, Sprite& edit_sprite) {
	int width = text.getLocalBounds().width + edit.field.getSize().x + 10;
	text.setPosition(x - width / 2 - 5, text.getPosition().y);
	if (text.getString() != "") {
		edit.setButtonPosition(Vector2f(text.getGlobalBounds().left + text.getGlobalBounds().width + 10, edit.field.getPosition().y));
		edit_sprite.setPosition(Vector2f(text.getGlobalBounds().left + text.getGlobalBounds().width + 10, edit.field.getPosition().y));
	} else {
		edit.setButtonPosition(Vector2f(690, 73));
		edit_sprite.setPosition(Vector2f(690, 73));
	}
}

int main() {
	RenderWindow window(VideoMode(1450, 1000), "cfAssist", Style::Close | Style::Titlebar);
	font.loadFromFile("CascadiaCode.ttf");

	Texture cf_logo_t, bar_t;
	cf_logo_t.loadFromFile("cf_logo.png");
	bar_t.loadFromFile("bar.png");
	cf_logo_t.setSmooth(true);
	bar_t.setSmooth(true);

	Sprite cf_logo, left_bar, right_bar;
	cf_logo.setTexture(cf_logo_t);
	left_bar.setTexture(bar_t);
	right_bar.setTexture(bar_t);
	cf_logo.setPosition(657, 650);
	left_bar.setPosition(75, 73);
	right_bar.setPosition(1280, 73);
	cf_logo.setScale(0.5, 0.5);
	left_bar.setScale(0.45, 0.5);
	right_bar.setScale(0.45, 0.5);

	Color back(24, 4, 36, 255);
	Color back_light(35, 5, 51, 255);
	Color solved(130, 209, 25, 255);
	Color solvedall(204, 204, 204, 255);
	Color text = Color::White;

	Texture edit_t;
	edit_t.loadFromFile("edit.png");
	edit_t.setSmooth(true);

	Sprite edit_sprite;
	edit_sprite.setTexture(edit_t);
	edit_sprite.setPosition(690, 73);
	edit_sprite.setScale(0.1, 0.1);

	Button edit({ edit_sprite.getLocalBounds().width * edit_sprite.getScale().x,
		edit_sprite.getLocalBounds().height * edit_sprite.getScale().y },
		{ edit_sprite.getPosition().x, edit_sprite.getPosition().y }, back);
	edit.setButtonColor(back, Color(255, 255, 255, 80));

	RectangleShape background;
	background.setSize({ 1450, 1000 });
	background.setFillColor(back);

	InputField left({ 200, 50 }, { 78, 890 }, back_light, text);
	InputField right({ 200, 50 }, { 1170, 890 }, back_light, text);
	right.backinsert = true;

	bool cf_available = true;

	Text subject, attempts_title, solved_title, solved_contest_title, stability_title, average_title;

	subject.setString("");
	attempts_title.setString("Attempts");
	solved_title.setString("Solved");
	solved_contest_title.setString("Solved on contest");
	stability_title.setString("stability");
	average_title.setString("Average rating");

	subject.setFont(font);
	attempts_title.setFont(font);
	solved_title.setFont(font);
	solved_contest_title.setFont(font);
	stability_title.setFont(font);
	average_title.setFont(font);

	subject.setCharacterSize(43);
	attempts_title.setCharacterSize(22);
	solved_title.setCharacterSize(22);
	solved_contest_title.setCharacterSize(22);
	stability_title.setCharacterSize(22);
	average_title.setCharacterSize(22);

	subject.setPosition({ 437, 70 });
	attempts_title.setPosition({ 662, 203 });
	solved_title.setPosition({ 672, 300 });
	solved_contest_title.setPosition({ 607, 387 });
	stability_title.setPosition({ 663, 810 });
	average_title.setPosition({ 627, 480 });

	Text left_attempts_v, left_solved_v, left_solved_contest_v, left_stability_v, left_average_v;

	left_attempts_v.setPosition({ 335, 203 });
	left_solved_v.setPosition({ 335, 300 });
	left_solved_contest_v.setPosition({ 335, 387 });
	left_stability_v.setPosition({ 335, 810 });
	left_average_v.setPosition({ 335, 480 });

	left_attempts_v.setFont(font);
	left_solved_v.setFont(font);
	left_solved_contest_v.setFont(font);
	left_stability_v.setFont(font);
	left_average_v.setFont(font);

	left_attempts_v.setCharacterSize(22);
	left_solved_v.setCharacterSize(22);
	left_solved_contest_v.setCharacterSize(22);
	left_stability_v.setCharacterSize(22);
	left_average_v.setCharacterSize(22);

	Text right_attempts_v, right_solved_v, right_solved_contest_v, right_stability_v, right_average_v, unavailable;

	right_attempts_v.setPosition({ 1096, 203 });
	right_solved_v.setPosition({ 1096, 300 });
	right_solved_contest_v.setPosition({ 1096, 387 });
	right_stability_v.setPosition({ 1096, 810 });
	right_average_v.setPosition({ 1096, 480 });
	unavailable.setPosition({ 285, 570 });

	right_attempts_v.setFont(font);	
	right_solved_v.setFont(font);
	right_solved_contest_v.setFont(font);
	right_stability_v.setFont(font);
	right_average_v.setFont(font);
	unavailable.setFont(font);

	right_attempts_v.setCharacterSize(22);
	right_solved_v.setCharacterSize(22);
	right_solved_contest_v.setCharacterSize(22);
	right_stability_v.setCharacterSize(22);
	right_average_v.setCharacterSize(22);
	unavailable.setCharacterSize(43);

	unavailable.setString("Codeforces is temporary unavailable");

	Line left_st({ 510, 845 }, { 510, 845 }, 2, text), right_st({ 935, 845 }, { 935, 845 }, 2, text);
	
	RectangleShape left_solved_contest, right_solved_contest, left_solved, right_solved;

	left_solved_contest.setFillColor(solved);
	right_solved_contest.setFillColor(solved);
	left_solved.setFillColor(solvedall);
	right_solved.setFillColor(solvedall);

	bool left_update = false, right_update = false;
		
	while (window.isOpen()) {
		window.clear();
		Event event;
		Vector2i mouse_position = Mouse::getPosition(window);
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
			left.change(event, mouse_position);
			right.change(event, mouse_position);
			if (edit.pressed(mouse_position, event)) {
				subject.setString(getSubject(subject.getString()));
				updateSubject(subject, 725, edit, edit_sprite);
				left_update = true;
				right_update = true;
			}
		}
		if (!left.isChosen() && left_update) {
			Participant now = makeUser(left.value);
			if (!now.GetCFStatus()) {
				cf_available = false;
			} else {
				cf_available = true;
			}
			int all = GetALL(now, subject.getString());
			int ok = GetOK(now, subject.getString());
			int ok_contest = GetContestOK(now, subject.getString());
			left_attempts_v.setString(std::to_string(all));
			left_solved_v.setString(std::to_string(ok));
			left_solved_contest_v.setString(std::to_string(ok_contest));
			float now_value1 = GetStability(now, subject.getString());
			if (now_value1 != INF) {
				left_stability_v.setString(std::to_string((int)round(now_value1 * 100)));
				left_st.setPoints(Vector2f(510, 835), Vector2f(510, 835 - 300 * now_value1));
			} else {
				left_stability_v.setString("");
				left_st.setPoints(Vector2f(510, 835), Vector2f(510, 835));
			}
			int now_value2 = GetAverageRating(now, subject.getString());
			if (now_value2 != INF) {
				left_average_v.setString(std::to_string(now_value2));
			} else {
				left_average_v.setString("");
			}

			left_solved.setPosition(left_bar.getGlobalBounds().left,
				left_bar.getGlobalBounds().top + (left_bar.getGlobalBounds().height - 5 -
					(left_bar.getGlobalBounds().height - 5) * ((1.0 * ok) / all)));
			left_solved_contest.setPosition(left_bar.getGlobalBounds().left,
				left_bar.getGlobalBounds().top + (left_bar.getGlobalBounds().height - 5 - 
					(left_bar.getGlobalBounds().height - 5) * ((1.0 * ok_contest) / all)));

			left_solved.setSize(Vector2f(left_bar.getGlobalBounds().width,
				(left_bar.getGlobalBounds().height - 5) * ((1.0 * ok) / all)));
			left_solved_contest.setSize(Vector2f(left_bar.getGlobalBounds().width, 
				(left_bar.getGlobalBounds().height - 5) * ((1.0 * ok_contest) / all)));

			left_update = false;
		} else if (left.isChosen()) {
			left_update = true;
		}
		if (!right.isChosen() && right_update) {
			float attempts_x, solved_x, solved_contest_x, stability_x, average_v;
			attempts_x = right_attempts_v.getPosition().x + right_attempts_v.getLocalBounds().width;
			solved_x = right_solved_v.getPosition().x + right_solved_v.getLocalBounds().width;
			solved_contest_x = right_solved_contest_v.getPosition().x + right_solved_contest_v.getLocalBounds().width;
			stability_x = right_stability_v.getPosition().x + right_stability_v.getLocalBounds().width;
			average_v = right_average_v.getPosition().x + right_average_v.getLocalBounds().width;

			Participant now = makeUser(right.value);
			if (!now.GetCFStatus()) {
				cf_available = false;
			} else {
				cf_available = true;
			}
			int all = GetALL(now, subject.getString());
			int ok = GetOK(now, subject.getString());
			int ok_contest = GetContestOK(now, subject.getString());
			right_attempts_v.setString(std::to_string(GetALL(now, subject.getString())));
			right_solved_v.setString(std::to_string(GetOK(now, subject.getString())));
			right_solved_contest_v.setString(std::to_string(GetContestOK(now, subject.getString())));
			float now_value1 = GetStability(now, subject.getString());
			if (now_value1 != INF) {
				right_stability_v.setString(std::to_string((int)round(now_value1 * 100)));
				right_st.setPoints(Vector2f(935, 835), Vector2f(935, 835 - 300 * now_value1));
			} else {
				right_stability_v.setString("");
				right_st.setPoints(Vector2f(935, 835), Vector2f(935, 835));
			}
			int now_value2 = GetAverageRating(now, subject.getString());
			if (now_value2 != INF) {
				right_average_v.setString(std::to_string(now_value2));
			} else {
				right_average_v.setString("");
			}

			right_attempts_v.setPosition(attempts_x - right_attempts_v.getLocalBounds().width,
				right_attempts_v.getPosition().y);
			right_solved_v.setPosition(solved_x - right_solved_v.getLocalBounds().width,
				right_solved_v.getPosition().y);
			right_solved_contest_v.setPosition(solved_contest_x - right_solved_contest_v.getLocalBounds().width,
				right_solved_contest_v.getPosition().y);
			right_stability_v.setPosition(stability_x - right_stability_v.getLocalBounds().width,
				right_stability_v.getPosition().y);
			right_average_v.setPosition(average_v - right_average_v.getLocalBounds().width,
				right_average_v.getPosition().y);

			right_solved.setPosition(right_bar.getGlobalBounds().left,
				right_bar.getGlobalBounds().top + (right_bar.getGlobalBounds().height - 5 -
					(right_bar.getGlobalBounds().height - 5) * ((1.0 * ok) / all)));
			right_solved_contest.setPosition(right_bar.getGlobalBounds().left,
				right_bar.getGlobalBounds().top + (right_bar.getGlobalBounds().height - 5 -
					(right_bar.getGlobalBounds().height - 5) * ((1.0 * ok_contest) / all)));

			right_solved.setSize(Vector2f(right_bar.getGlobalBounds().width,
				(right_bar.getGlobalBounds().height - 5) * ((1.0 * ok) / all)));
			right_solved_contest.setSize(Vector2f(right_bar.getGlobalBounds().width,
				(right_bar.getGlobalBounds().height - 5) * ((1.0 * ok_contest) / all)));

			right_update = false;
		} else if (right.isChosen()) {
			right_update = true;
		}

		window.draw(background);

		left.draw(window);
		right.draw(window);
		left_st.draw(window);
		right_st.draw(window);
		edit.draw(window);

		if (!cf_available) {
			window.draw(unavailable);
		}

		window.draw(left_attempts_v);
		window.draw(left_solved_v);
		window.draw(left_solved_contest_v);
		window.draw(left_stability_v);
		window.draw(left_average_v);
		window.draw(right_attempts_v);
		window.draw(right_solved_v);
		window.draw(right_solved_contest_v);
		window.draw(right_stability_v);
		window.draw(right_average_v);
		window.draw(edit_sprite);
		window.draw(left_solved);
		window.draw(left_solved_contest);
		window.draw(right_solved);
		window.draw(right_solved_contest);

		window.draw(subject);
		window.draw(attempts_title);
		window.draw(solved_title);
		window.draw(solved_contest_title);
		window.draw(stability_title);
		window.draw(average_title);
		window.draw(left_bar);
		window.draw(right_bar);
		window.draw(cf_logo);

		window.display();
	}
}
