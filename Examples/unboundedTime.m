% Example in matlab showing an unbounded execution-time example

function time = lastTimeAbove36C()
    while(true)
        currentTemperature = getCurrentTemperature();
        if currentTemperature > 36
            break
        end
        pause(0.1)
    end
    time = getCurrentTime();
end
