/// class Player - l'objet controllé
class Player {
  // Attributes
public:
   /*enum Action
		{
			MoveLeft,
			MoveRight,
			MoveUp,
			MoveDown,
			///ActionCou   enum MoveLeft, MoveRight, ...;*/
  std::map<sf::Keyboard::key, Action> mKeyBinding;
  std::map<Action, Command> mActionBinding;
  // Operations
public:
  /// Constructeur. c'est ici qu'on appel l'initializeActions() apres avoi defini la
  Player ();
  void handleEvent (const sf::Event& event, CommandQueue& commands);
  void handleRealTimeInput (CommandQueue& commands);
  void assignKey (Action action, sf::Keyboard::Key key);
  sf::Keyboard::Key getAssignedKey (action action);
private:
  /// association des instances du fonction template(derivedAction) avec la fonction
  void initializeActions ();
};

