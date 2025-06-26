import 'package:flutter/material.dart';
import 'package:the_court/datas/dev_start_data.dart';
import 'package:the_court/screen/contents/club/club_home_screen.dart';

class HomeScreen extends StatefulWidget {
  final DevStartData data;

  const HomeScreen({
    super.key,
    required this.data,
  });

  @override
  State<HomeScreen> createState() => _HomeScreenState();
}

class _HomeScreenState extends State<HomeScreen> {
  final List<BottomNavigationBarItem> _botItems = [
    BottomNavigationBarItem(
      icon: Icon(Icons.handshake),
      label: '클럽',
    ),
    BottomNavigationBarItem(
      icon: Icon(Icons.face),
      label: '파트너',
    ),
    BottomNavigationBarItem(
      icon: Icon(Icons.flag),
      label: '대회',
    ),
    BottomNavigationBarItem(
      icon: Icon(Icons.shopping_bag),
      label: '쇼핑',
    ),
  ];

  final List<Widget> _botWidget = [
    ClubHomeScreen(),
    Container(),
    Container(),
    Container(),
  ];

  final List<String> _titles = [
    '클럽',
    '파트너',
    '대회',
    '쇼핑',
  ];

  int _selectedBotIdx = 0;

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(
          _titles[_selectedBotIdx],
          style: TextStyle(
            color: Colors.indigo,
          ),
        ),
      ),
      body: _botWidget[_selectedBotIdx],
      bottomNavigationBar: _bottomNavBar(),
    );
  }


  Widget _bottomNavBar()
  {
    return BottomNavigationBar(
      type: BottomNavigationBarType.fixed,
      items: _botItems,
      selectedItemColor: Colors.indigo,
      currentIndex: _selectedBotIdx,
      onTap: (index) {
        setState(() {
          _selectedBotIdx = index;
        });
      },
    );
  }
}
