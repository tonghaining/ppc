code = "extra"
name = "X"
descr = "extra"


def html():
    from markupsafe import Markup
    return Markup(f"""
<p>Here you will find some free-to-use tasks you can use for experimentation.</p>

<p>Here you will also find some extra opportunities for getting additional points by helping us with improving the course. But even if you do not need the points, please nevertheless provide feedback on our course and its arrangements!</p>

""")


def explain_web(test):
    raise NotImplementedError


def explain_terminal(output, color=False):
    raise NotImplementedError
