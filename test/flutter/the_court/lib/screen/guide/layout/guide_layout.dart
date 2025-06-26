import 'package:flutter/material.dart';

class GuideBodyLayout extends StatelessWidget {

  final String nxtBtnTxt;
  final String mainTxt;
  final String? subTxt;
  final VoidCallback? onNxtBtnPressed;
  final Widget? top;
  final Widget? bottom;

  const GuideBodyLayout({
    super.key,
    required this.mainTxt,
    this.subTxt,
    required this.nxtBtnTxt,
    required this.onNxtBtnPressed,
    this.top,
    this.bottom,
  });

  @override
  Widget build(BuildContext context) {
    double boxHeight = MediaQuery.of(context).size.height * 0.25;

    return GestureDetector(
      onTap: () {
        FocusScope.of(context).unfocus();
      },
      child: SafeArea(
        child: Padding(
          padding: const EdgeInsets.all(16.0),
          child: SingleChildScrollView(
            physics: PageScrollPhysics(),
            scrollDirection: Axis.vertical,
            child: Column(
              mainAxisAlignment: MainAxisAlignment.start,
              crossAxisAlignment: CrossAxisAlignment.stretch,
              children: [
                _Header(
                  height: boxHeight,
                  mainTxt: mainTxt,
                  subTxt: subTxt,
                ),
                _Body(
                  height: boxHeight,
                  body: top,
                ),
                _Body(
                  height: boxHeight,
                  body: bottom,
                ),
                _Footer(
                  height: boxHeight,
                  onPressed: onNxtBtnPressed,
                  btnStr: nxtBtnTxt,
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}


class _Header extends StatelessWidget {
  final double height;
  final String? mainTxt;
  final String? subTxt;

  const _Header({
    super.key,
    required this.height,
    this.mainTxt,
    this.subTxt,
  });

  @override
  Widget build(BuildContext context) {
    String mainStr = mainTxt == null ? '' : mainTxt!;
    String subStr = subTxt == null ? '' : subTxt!;

    return SizedBox(
      height: height,
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.stretch,
        children: [
          _MainText(str: mainStr),
          _SubText(str: subStr),
        ],
      ),
    );
  }
}


class _MainText extends StatelessWidget {

  final String str;

  const _MainText({
    super.key,
    required this.str
  });

  @override
  Widget build(BuildContext context) {
    return Text(
      str,
      style: const TextStyle(
        color: Colors.indigo,
        fontSize: 40.0,
        fontWeight: FontWeight.w500,
      ),
    );
  }
}


class _SubText extends StatelessWidget {
  final String str;

  const _SubText({
    super.key,
    required this.str,
  });

  @override
  Widget build(BuildContext context) {
    return Text(
      str,
      style: const TextStyle(
        color: Colors.indigo,
        fontSize: 25.0,
        fontWeight: FontWeight.w500,
      ),
    );
  }
}


class _Body extends StatelessWidget {
  final double height;
  Widget? body;

  _Body({
    super.key,
    required this.height,
    this.body,
  });

  @override
  Widget build(BuildContext context) {
    return SizedBox(
      height: height,
      child: body == null ? Container() : body!,
    );
  }
}


class _Footer extends StatelessWidget {
  final double height;
  String? btnStr;
  VoidCallback? onPressed;

  _Footer({
    super.key,
    required this.height,
    this.btnStr,
    this.onPressed,
  });

  @override
  Widget build(BuildContext context) {
    return SizedBox(
      height: height,
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.stretch,
        children: [
          ElevatedButton(
            onPressed: onPressed,
            style: ElevatedButton.styleFrom(
              backgroundColor: Colors.indigo,
              foregroundColor: Colors.white,
            ),
            child: Text(btnStr == null ? '' : btnStr!),
          ),
        ],
      ),
    );
  }
}


