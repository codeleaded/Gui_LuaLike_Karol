from io import *
from sys import *
from window import *
from obj import *
from math import *


class vec2
    function new(px,py)
        m = obj::new("vec2")
        m.x = px
        m.y = py
        return m
    end
end

function overlap(rect1p,rect1d,rect2p,rect2d)
    if rect1p.x < (rect2p.x - rect1d.x)
        return false
    elif rect1p.x >= (rect2p.x + rect2d.x)
        return false
    elif rect1p.y < (rect2p.y - rect1d.y)
        return false
    elif rect1p.y >= (rect2p.y + rect2d.y)
        return false        
    end
    return true
end

function main()
    w = window::new("Hello World",1900,1000)
    w.init()

    PADDLEWIDTH = 0.03
    PADDLEHEIGHT = 0.2
    BALLWIDTH = 0.027
    BALLHEIGHT = 0.05

    score1 = 0
    score2 = 0

    rect1 = vec2::new(0.0,0.5)
    rect2 = vec2::new(1.0 - PADDLEWIDTH,0.5)
    ball = vec2::new(0.5,0.5)
    ballvelo = vec2::new(-0.2,0.2)

    while w.running()
        w.update()

        if w.key("W","down")
            rect1.y = rect1.y - 0.5 * w.elapsed()
        elif w.key("S","down")
            rect1.y = rect1.y + 0.5 * w.elapsed()
        end

        if w.key("UP","down")
            rect2.y = rect2.y - 0.5 * w.elapsed()
        elif w.key("DOWN","down")
            rect2.y = rect2.y + 0.5 * w.elapsed()
        end

        ball.x = ball.x + ballvelo.x * w.elapsed()
        ball.y = ball.y + ballvelo.y * w.elapsed()

        if rect1.y < 0.0
            rect1.y = 0.0
        end
        if rect1.y > (1.0 - PADDLEHEIGHT)
            rect1.y = 1.0 - PADDLEHEIGHT
        end
        if rect2.y < 0.0
            rect2.y = 0.0
        end
        if rect2.y > (1.0 - PADDLEHEIGHT)
            rect2.y = 1.0 - PADDLEHEIGHT
        end

        if ball.y < 0.0
            ball.y = 0.0
            ballvelo.y = -ballvelo.y * 1.05
        end
        if ball.y > (1.0 - BALLHEIGHT)
            ball.y = 1.0 - BALLHEIGHT
            ballvelo.y = -ballvelo.y * 1.05
        end
        if ball.x < (-PADDLEWIDTH)
            ball.x = 0.5
            ball.y = 0.5
            ballvelo.x = 0.2
            ballvelo.y = 0.2
            score2 = score2 + 1
        end
        if ball.x > 1.0
            ball.x = 0.5
            ball.y = 0.5
            ballvelo.x = 0.2
            ballvelo.y = 0.2
            score1 = score1 + 1
        end

        -- if rect1.x >= (ball.x - PADDLEWIDTH) && rect1.x < (ball.x + BALLWIDTH) && rect1.y >= (ball.y - PADDLEHEIGHT) && rect1.y < (ball.y + BALLHEIGHT)
        --     ballvelo.x = -ballvelo.x * 1.1
        -- end
        if overlap(rect1,vec2::new(PADDLEWIDTH,PADDLEHEIGHT),ball,vec2::new(BALLWIDTH,BALLHEIGHT))
            ballvelo.x = -ballvelo.x * 1.1
        end
        if rect2.x >= (ball.x - PADDLEWIDTH) && rect2.x < (ball.x + BALLWIDTH) && rect2.y >= (ball.y - PADDLEHEIGHT) && rect2.y < (ball.y + BALLHEIGHT)
            ballvelo.x = -ballvelo.x * 1.1
        end


        w.clear(0xFF000000)
        
        wwidth = w.width()
        wheight = w.height()
        w.rect(rect1.x * wwidth,rect1.y * wheight,PADDLEWIDTH * wwidth,PADDLEHEIGHT * wheight,0xFF0000FF)
        w.rect(rect2.x * wwidth,rect2.y * wheight,PADDLEWIDTH * wwidth,PADDLEHEIGHT * wheight,0xFFFF0000)
        w.rect(ball.x * wwidth,ball.y * wheight,BALLWIDTH * wwidth,BALLHEIGHT * wheight,0xFFFFFFFF)

        sc1 = math::getInt(score1)
        sc2 = math::getInt(score2)
        w.string(sc1+" : "+sc2,0.5 * wwidth,0.01 * wheight,0xFFFFFFFF)

        w.render()
        sys::msleep(5)
    end

    return 0
end