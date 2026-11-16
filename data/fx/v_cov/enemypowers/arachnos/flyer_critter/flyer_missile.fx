#########################################################
##	
##

FxInfo
LifeSpan  200

Input  
	InpName	Target
End

Input  
	InpName	Mystic
End

Input  
	InpName	WepR
End

##################################



Condition
	On	Time
	Time	20
	Event
		Type	Local
		At	origin
		Sound rocketlaunch 200 200 .69
	End
End

Condition
	On	Time
	Time	25
	Event
		Type	Local
		At	origin
		Sound rocketlaunch2 200 200 .66
	End
End


Condition
	On	Time
	Time	30
	Event
		Type	Local
		At	origin
		Sound rocketlaunch3 200 200 .55
	End
End

Condition
	On	Time
	Time	35
	Event
		Type	Local
		At	origin
		Sound rocketlaunch2 200 200 .69
	End
End


Condition
	On 	Time
	Time 	20
	
	Event
		ename	prime
		Type	startpositonly
		At 	origin
#		geom	malta_rocket_grey #minimissile canister
		magnet	target
		bhvroverride
			trackrate 2.7
		end
		
	End

	Event
		ename	launcherL
		Type	local
		At 	hips
		bhvroverride
			positionoffset 1.2 -.71 1.0 #1.28 -.51 1.3
			pyrrotate 0 0 -35
		end
		
	End


	Event
		ename	launcherR
		Type	local
		At 	hips
		bhvroverride
			positionoffset -1.2 -.71 1.0
			pyrrotate 0 0 35
		end
		
	End

end

Condition
	On 	Time
	Time 	20

	

	Event
		ename	miss1R
		Type	local
		At 	launcherR
		childfx	:flyer_missile_single.fx
		bhvroverride
			pyrrotate 0 0 -90
		end
		
	End
	Event
		ename	miss1L
		Type	local
		At 	launcherL
		childfx	:flyer_missile_single.fx
		bhvroverride
			pyrrotate 0 0 90
		end
		
	End
	
	Event
                Type 	SetLight
                LifeSpan 	5
		bhvroverride
			PulsePeakTime		0
			PulseBrightness		1
		end
        End
End

Condition
	On 	Time
	Time 	25


	Event
		ename	miss2R
		Type	local
		At 	launcherR
		childfx	:flyer_missile_single.fx
		bhvroverride
			pyrrotate 0 0 -60
		end
		
	End
	Event
		ename	miss2L
		Type	local
		At 	launcherL
		childfx	:flyer_missile_single.fx
		bhvroverride
			pyrrotate 0 0 60
		end
		
	End
	
	Event
                Type 	SetLight
                LifeSpan 	5
		bhvroverride
			PulsePeakTime		0
			PulseBrightness		1
		end
        End
End

Condition
	On 	Time
	Time 	30

	Event
		ename	miss3R
		Type	local
		At 	launcherR
		childfx	:flyer_missile_single.fx
		bhvroverride
			pyrrotate 0 0 -30
		end
		
	End
	Event
		ename	miss3L
		Type	local
		At 	launcherL
		childfx	:flyer_missile_single.fx
		bhvroverride
			pyrrotate 0 0 30
		end
		
	End
	
	Event
                Type 	SetLight
                LifeSpan 	5
		bhvroverride
			PulsePeakTime		0
			PulseBrightness		1
		end
        End
End

Condition
	On 	Time
	Time 	35


	Event
		ename	miss4R
		Type	local
		At 	launcherR
		childfx	:flyer_missile_single.fx
		bhvroverride
			pyrrotate 0 0 -75
		end
		
	End
	Event
		ename	miss4L
		Type	local
		At 	launcherL
		childfx	:flyer_missile_single.fx
		bhvroverride
			pyrrotate 0 0 75
		end
		
	End
	
	Event
                Type 	SetLight
                LifeSpan 	5
		bhvroverride
			PulsePeakTime		0
			PulseBrightness		1
		end
        End
End



Condition
	On 	Time
	Time 	40


	Event
		ename	miss5R
		Type	local
		At 	launcherR
		childfx	:flyer_missile_single.fx
		bhvroverride
			pyrrotate 0 0 -45
		end
		
	End
	Event
		ename	miss5L
		Type	local
		At 	launcherL
		childfx	:flyer_missile_single.fx
		bhvroverride
			pyrrotate 0 0 45
		end
		
	End
	
	Event
                Type 	SetLight
                LifeSpan 	5
		bhvroverride
			PulsePeakTime		0
			PulseBrightness		1
		end
        End
End

end
