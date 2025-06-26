import 'package:flutter/material.dart';

class ContentHomeLayout extends StatelessWidget {
  final Widget body;

  const ContentHomeLayout({
    super.key,
    required this.body,
  });

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: EdgeInsets.symmetric(horizontal: 8.0),
      child: body,
    );
  }
}

class AdBanner extends StatelessWidget {
  const AdBanner({super.key});

  @override
  Widget build(BuildContext context) {
    return SizedBox(
      width: 320.0,
      height: 50.0,
      child: Container(
        color: Colors.grey,
        child: Text('광고...'),
      ),
    );
  }
}

class SubjectText extends StatelessWidget {
  final String subject;

  const SubjectText({
    super.key,
    required this.subject,
  });

  @override
  Widget build(BuildContext context) {
    return Text(
      subject,
      style: TextStyle(
        color: Colors.indigo,
        fontSize: 16.0,
        fontWeight: FontWeight.w700,
      ),
    );
  }
}

