#########################################################
##	
##
FxInfo

LifeSpan  300

#######################################################
######################## 1 ############################
#######################################################

Condition
	On	Time
	Time	9
	Event
		Type	Local
		At	Rwing2
		Sound Servodoor5 200 200 .79
	End
End


Condition
	On	Time
	Time	40
	Event
		Type	Local
		At	Rwing2
		Sound rocketlaunch 200 200 .69
	End
End

Condition
	On	Time
	Time	51
	Event
		Type	Local
		At	Rwing2
		Sound rocketlaunch3 200 200 .66
	End
End


Condition
	On	Time
	Time	62
	Event
		Type	Local
		At	Rwing2
		Sound rocketlaunch2 200 200 .55
	End
End

############### Fire First Two Rockets, and Explode on impact ####################
Condition
	On	Time
	Time	40

	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	:RocketLights.bhvr
		LifeSpan 5
	End

	Event
		EName 	Prime 
		Type	start
		At	Lwing2
		Bhvr	:TitanRocketSmall1.bhvr
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	Offset1 
		Type	local
		At	Prime
		Bhvr	:TitanRocketOffset.bhvr
		Part	:RocketGlow1.part
		Part	:RocketFire1.part
		Part	:Rocketsmoke.part
		Geom	Malta_Rocket_Grey
	End

	Event
		EName 	RocketFlash1
		Type	local
		At	Offset1
		Bhvr	:TitanRocketOffset.bhvr
		Part	:RocketFlash1.part
		Lifespan 5
	End

	Event
		EName 	Offset2 
		Type	local
		At	Offset1
		Bhvr	:TitanRocketOffset2.bhvr
		Part	:RocketGlow1.part
		Part	:RocketFire1.part
		Part	:Rocketsmoke.part
		Geom	Malta_Rocket_Grey
	End

End

Condition
	On	PrimeHit

	Event
		EName	Explosion1
		Type	Start
		At	T_Chest
		Part	:WhiteCenter.part
		Part	:Flash01.part
		Part	:Smoke1.part
		Part	:Smoke2.part
		bhvr	:RocketExploShake2.bhvr
		Sound flashgrenade 100 100 .88
		Lifespan 5		
	End

	Event
		Ename	Prime
		Type	Destroy
	End
End

############### Fire Second Two Rockets, and Explode on impact  ####################
Condition
	On	Time
	Time	46

	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	:RocketLights.bhvr
		LifeSpan 5
	End

	Event
		EName 	Prime2 
		Type	start
		At	Rwing2
		Bhvr	:TitanRocketSmall2.bhvr
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	Offset3 
		Type	local
		At	Prime2
		Bhvr	:TitanRocketOffset3.bhvr
		Part	:RocketGlow1.part
		Part	:RocketFire1.part
		Part	:Rocketsmoke.part
		Geom	Malta_Rocket_Grey
	End

	Event
		EName 	RocketFlash1
		Type	local
		At	Offset3
		Bhvr	:TitanRocketOffset.bhvr
		Part	:RocketFlash1.part
		Lifespan 5
	End

	Event
		EName 	Offset4 
		Type	local
		At	Offset3
		Bhvr	:TitanRocketOffset2.bhvr
		Part	:RocketGlow1.part
		Part	:RocketFire1.part
		Part	:Rocketsmoke.part
		Geom	Malta_Rocket_Grey
	End

End

Condition
	On	Prime2Hit

	Event
		EName	Explosion2
		Type	Start
		At	T_Chest
		Part	:WhiteCenter.part
		Part	:Flash01.part
		Part	:Smoke1.part
		Part	:Smoke2.part
		bhvr	:RocketExploShake2.bhvr
		Sound grexplo2 100 100 .75
		Lifespan 5		
	End

	Event
		Ename	Prime2
		Type	Destroy
	End
End

############### Fire Third Two Rockets, and Explode on impact  ####################

Condition
	On	Time
	Time	52

	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	:RocketLights.bhvr
		LifeSpan 5
	End

	Event
		EName 	Prime3 
		Type	start
		At	Lwing2
		Bhvr	:TitanRocketSmall1.bhvr
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	Offset5 
		Type	local
		At	Prime3
		Bhvr	:TitanRocketOffset.bhvr
		Part	:RocketGlow1.part
		Part	:RocketFire1.part
		Part	:Rocketsmoke.part
		Geom	Malta_Rocket_Grey
	End

	Event
		EName 	RocketFlash1
		Type	local
		At	Offset5
		Bhvr	:TitanRocketOffset.bhvr
		Part	:RocketFlash1.part
		Lifespan 5
	End

	Event
		EName 	Offset6 
		Type	local
		At	Offset5
		Bhvr	:TitanRocketOffset2.bhvr
		Part	:RocketGlow1.part
		Part	:RocketFire1.part
		Part	:Rocketsmoke.part
		Geom	Malta_Rocket_Grey
	End

End

Condition
	On	Prime3Hit

	Event
		EName	Explosion3
		Type	Start
		At	T_Chest
		Part	:WhiteCenter.part
		Part	:Flash01.part
		Part	:Smoke1.part
		bhvr	:RocketExploShake2.bhvr
		Part	:Smoke2.part
		Sound grexplo4 100 100 .75
		Lifespan 5		
	End

	Event
		Ename	Prime3
		Type	Destroy
	End
End


############### Fire Fourth Two Rockets, and Explode on impact  ####################
Condition
	On	Time
	Time	58

	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	:RocketLights.bhvr
		LifeSpan 5
	End

	Event
		EName 	Prime4 
		Type	start
		At	Rwing2
		Bhvr	:TitanRocketSmall2.bhvr
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	Offset7 
		Type	local
		At	Prime4
		Bhvr	:TitanRocketOffset3.bhvr
		Part	:RocketGlow1.part
		Part	:RocketFire1.part
		Part	:Rocketsmoke.part
		Geom	Malta_Rocket_Grey
	End

	Event
		EName 	RocketFlash1
		Type	local
		At	Offset7
		Bhvr	:TitanRocketOffset.bhvr
		Part	:RocketFlash1.part
		Lifespan 5
	End

	Event
		EName 	Offset8
		Type	local
		At	Offset7
		Bhvr	:TitanRocketOffset2.bhvr
		Part	:RocketGlow1.part
		Part	:RocketFire1.part
		Part	:Rocketsmoke.part
		Geom	Malta_Rocket_Grey
	End

End

Condition
	On	Prime4Hit

	Event
		EName	Explosion4
		Type	Start
		At	T_Chest
		Part	:WhiteCenter.part
		Part	:Flash01.part
		Part	:Smoke1.part
		Part	:Smoke2.part
		bhvr	:RocketExploShake2.bhvr
		Sound explohit2 100 100 .8
		Lifespan 5		
	End

	Event
		Ename	Prime4
		Type	Destroy
	End
End


End				