#########################################################
##	
##
FxInfo

Input  
	InpName	Rwing2
End

Input  
	InpName	Lwing2
End

LifeSpan  300

#######################################################
######################## 1 ############################
#######################################################



Condition
	On	Time
	Time	10
	Event
		Type	Local
		At	Rwing2
		Sound Servodoor4 200 200 .77
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



Condition
	On Time
	Time 40
	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	:RocketLights.bhvr
		LifeSpan 5
	End

End

Condition
	On Time
	Time 46
	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	:RocketLights.bhvr
		LifeSpan 5
	End

End

Condition
	On Time
	Time 52
	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	:RocketLights.bhvr
		LifeSpan 5
	End

End

Condition
	On Time
	Time 58
	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	:RocketLights.bhvr
		LifeSpan 5
	End

End

Condition
	On Time
	Time 10
	Event
		EName 	RocketRig
		Type	Local
		At	Chest
		Geom	Malta_Rocket_Rig_Orange16
		LifeSpan 40
	End

End

Condition
	On Time
	Time 40
	Event
		EName 	RocketRig
		Type	Local
		At	Chest
		Geom	Malta_Rocket_Rig_Orange8
		LifeSpan 100
	End

End


Condition
	On	Time
	Time	40
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
		Geom	Malta_Rocket_Orange
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
		Geom	Malta_Rocket_Orange
	End

End

Condition
	On	PrimeHit
	Time	0

	Event
		Type	Local
		At	target
		Sound FireExplo 100 100 .85
	End
End


Condition
	On 	PrimeHit


	Event
		EName	Explosion1
		Type	Start
		At	T_Chest
		Part	:WhiteCenter.part
		Part	:Flash01.part
		Part	:Smoke1.part
		bhvr	:RocketExploShake2.bhvr
		Lifespan 5		
	End
End

Condition
	On 	PrimeHit


	Event
		EName	Smoke
		Type	Start
		At	T_Chest
		Part	:Smoke2.part
		Lifespan 5
	End

	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		Ename	Explosion
		Type	Destroy
	End

End

Condition
	On	Time
	Time	46
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
		Geom	Malta_Rocket_Orange
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
		Geom	Malta_Rocket_Orange
	End

End

Condition
	On	Prime2Hit


	Event
		Type	Local
		At	target
		Sound grexplo4 100 100 .85
	End
End


Condition
	On 	Prime2Hit
	Time 	0

	Event
		EName	Explosion2
		Type	Start
		At	T_Chest
		Part	:WhiteCenter.part
		Part	:Flash01.part
		Part	:Smoke1.part
		bhvr	:RocketExploShake2.bhvr
		Lifespan 5		
	End
End

Condition
	On 	Prime2Hit


	Event
		EName	Smoke
		Type	Start
		At	T_Chest
		Part	:Smoke2.part
		Lifespan 5
	End

	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		Ename	Explosion1
		Type	Destroy
	End

	Event
		Ename	Prime2
		Type	Destroy
	End

	Event
		Ename	Explosion2
		Type	Destroy
	End

End

#######################################################
######################## 2 ############################
#######################################################

Condition
	On	Time
	Time	52
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
		Geom	Malta_Rocket_Orange
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
		Geom	Malta_Rocket_Orange
	End

End

Condition
	On	Prime3Hit
	Time	0

	Event
		Type	Local
		At	target
		Sound flashgrenade 100 100 .75
	End
End


Condition
	On 	Prime3Hit


	Event
		EName	Explosion3
		Type	Start
		At	T_Chest
		Part	:WhiteCenter.part
		Part	:Flash01.part
		Part	:Smoke1.part
		bhvr	:RocketExploShake2.bhvr
		Lifespan 5		
	End
End

Condition
	On 	Prime3Hit


	Event
		EName	Smoke
		Type	Start
		At	T_Chest
		Part	:Smoke2.part
		Lifespan 5
	End

	Event
		Ename	Prime3
		Type	Destroy
	End

	Event
		Ename	Explosion3
		Type	Destroy
	End

End

Condition
	On	Time
	Time	58
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
		Geom	Malta_Rocket_Orange
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
		Geom	Malta_Rocket_Orange
	End

End

Condition
	On	Prime4Hit
	Time	0

	Event
		Type	Local
		At	target
		Sound smokegrexplo 100 100 .8
	End
End


Condition
	On 	Prime4Hit
	Time 	0

	Event
		EName	Explosion4
		Type	Start
		At	T_Chest
		Part	:WhiteCenter.part
		Part	:Flash01.part
		Part	:Smoke1.part
		bhvr	:RocketExploShake2.bhvr
		Lifespan 5		
	End
End

Condition
	On 	Prime4Hit

	Event
		EName	Smoke
		Type	Start
		At	T_Chest
		Part	:Smoke2.part
		Lifespan 5
	End

	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		Ename	Explosion1
		Type	Destroy
	End

	Event
		Ename	Prime4
		Type	Destroy
	End

	Event
		Ename	Explosion4
		Type	Destroy
	End

End


End				