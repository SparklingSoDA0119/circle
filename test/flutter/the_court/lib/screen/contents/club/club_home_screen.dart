import 'package:flutter/material.dart';
import 'package:the_court/screen/contents/layout/content_home_layout.dart';

class ClubHomeScreen extends StatefulWidget {
  const ClubHomeScreen({super.key});

  @override
  State<ClubHomeScreen> createState() => _ClubHomeScreenState();
}

class _ClubHomeScreenState extends State<ClubHomeScreen> {
  @override
  Widget build(BuildContext context) {
    return ContentHomeLayout(
      body: Column(
        crossAxisAlignment: CrossAxisAlignment.stretch,
        children: [
          Divider(),

          AdBanner(),

          Divider(),

          _JoinedClubView(),

        ],
      )
    );
  }
}

class _JoinedClubView extends StatelessWidget {
  const _JoinedClubView({super.key});

  @override
  Widget build(BuildContext context) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.stretch,
      children: [
        SubjectText(subject: '내 클럽'),

        Card(
          child: SizedBox(
            width: double.infinity,
            height: 200.0,
            child: _JoinedClubListView(),
          ),
        )
      ],
    );
  }
}


class _JoinedClubListView extends StatelessWidget {
  const _JoinedClubListView({super.key});

  @override
  Widget build(BuildContext context) {
    return ListView(
      physics: PageScrollPhysics(),
      scrollDirection: Axis.horizontal,
      children: [
        Row(
          children: getJoinedClubItems(),
        )
      ],
    );
  }

  List<Widget> getJoinedClubItems()
  {
    List<Widget> result = [];

    result.add(BasisJoinedClubItem(
      label: '클럽 찾기',
      icon: Icons.search,
      onPressed: () {},
    ));
    return result;
  }
}


class BasisJoinedClubItem extends StatelessWidget {
  final String label;
  final IconData icon;
  final VoidCallback onPressed;

  const BasisJoinedClubItem({
    super.key,
    required this.label,
    required this.icon,
    required this.onPressed,
  });

  @override
  Widget build(BuildContext context) {
    return SizedBox(
      height: 160.0,
      child: ElevatedButton(
        onPressed: onPressed,
        style: ElevatedButton.styleFrom(
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(10.0)
          ),
        ),
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Icon(icon, size: 50.0,),
            Text(label),
          ],
        ),
      ),
    );
  }
}



class _JoinedClubListItem extends StatelessWidget {
  final VoidCallback? onPressed;

  const _JoinedClubListItem({
    super.key,
    required this.onPressed,
  });

  @override
  Widget build(BuildContext context) {
    return SizedBox(
      width: 140.0,
      height: 160.0,
      child: ElevatedButton(
        style: ElevatedButton.styleFrom(
          padding: EdgeInsets.all(1.0),
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(10.0),
          )
        ),
        onPressed: onPressed,
        child: Padding(
          padding: const EdgeInsets.all(4.0),
          child: Column(
            mainAxisAlignment: MainAxisAlignment.start,
            children: [
              Image.asset(
                'assets/images/no-image.png',
                width: 100,
              ),
            ],
          ),
        )

      ),
    );
  }
}





