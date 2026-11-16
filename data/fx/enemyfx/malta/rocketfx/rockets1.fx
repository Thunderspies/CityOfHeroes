#########################################################
##	
##
FxInfo


LifeSpan  100

#######################################################
######################## 1 ############################
#######################################################

Condition
	On Time
	Time 1
	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	:RocketLights.bhvr
		LifeSpan 5
	End

End

Condition
	On Time
	Time 8
	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	:RocketLights.bhvr
		LifeSpan 5
	End

End

Condition
	On Time
	Time 16
	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	:RocketLights.bhvr
		LifeSpan 5
	End

End

Condition
	On Time
	Time 24
	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	:RocketLights.bhvr
		LifeSpan 5
	End

End

Condition
	On Time
	Time 30
	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	:RocketLights.bhvr
		LifeSpan 5
	End

End

Condition
	On Time
	Time 36
	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	:RocketLights.bhvr
		LifeSpan 5
	End

End

Condition
	On Time
	Time 42
	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	:RocketLights.bhvr
		LifeSpan 5
	End

End



Condition
	On	Time
	Time	0
	Event
		EName 	Prime 
		Type	start
		At	WepL
		Bhvr	:TitanRocket1.bhvr
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
		Geom	Malta_Rocket_Yellow
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
		Geom	Malta_Rocket_Yellow
	End

End

Condition
	On	PrimeHit
	Time	0

	Event
		Type	Local
		At	Origin
		#Sound	Effigy5 100 90 .75
	End
End


Condition
	On 	PrimeHit
	Time 	0

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
	Time 	5

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
	Time	6
	Event
		EName 	Prime2 
		Type	start
		At	WepR
		Bhvr	:TitanRocket2.bhvr
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
		Geom	Malta_Rocket_Yellow
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
		Geom	Malta_Rocket_Yellow
	End

End

Condition
	On	Prime2Hit
	Time	0

	Event
		Type	Local
		At	Origin
		#Sound	Effigy5 100 90 .75
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
	Time 	5

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
	Time	12
	Event
		EName 	Prime3 
		Type	start
		At	WepL
		Bhvr	:TitanRocket1.bhvr
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
		Geom	Malta_Rocket_Yellow
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
		Geom	Malta_Rocket_Yellow
	End

End

Condition
	On	PrimeHit
	Time	0

	Event
		Type	Local
		At	Origin
		#Sound	Effigy5 100 90 .75
	End
End


Condition
	On 	Prime3Hit
	Time 	0

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
	Time 	5

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
	Time	18
	Event
		EName 	Prime4 
		Type	start
		At	WepR
		Bhvr	:TitanRocket2.bhvr
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
		Geom	Malta_Rocket_Yellow
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
		Geom	Malta_Rocket_Yellow
	End

End

Condition
	On	Prime4Hit
	Time	0

	Event
		Type	Local
		At	Origin
		#Sound	Effigy5 100 90 .75
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
	Time 	5

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

#######################################################
######################## 3 ############################
#######################################################

Condition
	On	Time
	Time	24
	Event
		EName 	Prime5 
		Type	start
		At	WepL
		Bhvr	:TitanRocket1.bhvr
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	Offset9 
		Type	local
		At	Prime5
		Bhvr	:TitanRocketOffset.bhvr
		Part	:RocketGlow1.part
		Part	:RocketFire1.part
		Part	:Rocketsmoke.part
		Geom	Malta_Rocket_Yellow
	End

	Event
		EName 	RocketFlash1
		Type	local
		At	Offset9
		Bhvr	:TitanRocketOffset.bhvr
		Part	:RocketFlash1.part
		Lifespan 5
	End

	Event
		EName 	Offset10 
		Type	local
		At	Offset9
		Bhvr	:TitanRocketOffset2.bhvr
		Part	:RocketGlow1.part
		Part	:RocketFire1.part
		Part	:Rocketsmoke.part
		Geom	Malta_Rocket_Yellow
	End

End

Condition
	On	Prime5Hit
	Time	0

	Event
		Type	Local
		At	Origin
		#Sound	Effigy5 100 90 .75
	End
End


Condition
	On 	Prime5Hit
	Time 	0

	Event
		EName	Explosion5
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
	On 	Prime5Hit
	Time 	5

	Event
		EName	Smoke
		Type	Start
		At	T_Chest
		Part	:Smoke2.part
		Lifespan 5
	End

	Event
		Ename	Prime5
		Type	Destroy
	End

	Event
		Ename	Explosion5
		Type	Destroy
	End

End

Condition
	On	Time
	Time	30
	Event
		EName 	Prime6
		Type	start
		At	WepR
		Bhvr	:TitanRocket2.bhvr
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	Offset11 
		Type	local
		At	Prime6
		Bhvr	:TitanRocketOffset3.bhvr
		Part	:RocketGlow1.part
		Part	:RocketFire1.part
		Part	:Rocketsmoke.part
		Geom	Malta_Rocket_Yellow
	End

	Event
		EName 	RocketFlash1
		Type	local
		At	Offset11
		Bhvr	:TitanRocketOffset.bhvr
		Part	:RocketFlash1.part
		Lifespan 5
	End

	Event
		EName 	Offset12
		Type	local
		At	Offset11
		Bhvr	:TitanRocketOffset2.bhvr
		Part	:RocketGlow1.part
		Part	:RocketFire1.part
		Part	:Rocketsmoke.part
		Geom	Malta_Rocket_Yellow
	End

End

Condition
	On	Prime6Hit
	Time	0

	Event
		Type	Local
		At	Origin
		#Sound	Effigy5 100 90 .75
	End
End


Condition
	On 	Prime6Hit
	Time 	0

	Event
		EName	Explosion6
		Type	Start
		At	T_Chest
		Part	:WhiteCenter.part
		Part	:Flash01.part
		Part	:Smoke1.part
		bhvr	:RocketExploShake2.bhvr
		Lifespan 5		
	End

	Event
		Ename	Prime6
		Type	Destroy
	End

	Event
		Ename	Explosion6
		Type	Destroy
	End
End

Condition
	On 	Prime6Hit
	Time 	5

	Event
		EName	Smoke
		Type	Start
		At	T_Chest
		Part	:Smoke2.part
		Lifespan 5
	End

End


#######################################################
######################## 4 ############################
#######################################################

Condition
	On	Time
	Time	36
	Event
		EName 	Prime7 
		Type	start
		At	WepL
		Bhvr	:TitanRocket1.bhvr
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	Offset13
		Type	local
		At	Prime7
		Bhvr	:TitanRocketOffset.bhvr
		Part	:RocketGlow1.part
		Part	:RocketFire1.part
		Part	:Rocketsmoke.part
		Geom	Malta_Rocket_Yellow
	End

	Event
		EName 	RocketFlash1
		Type	local
		At	Offset13
		Bhvr	:TitanRocketOffset.bhvr
		Part	:RocketFlash1.part
		Lifespan 5
	End

	Event
		EName 	Offset14 
		Type	local
		At	Offset13
		Bhvr	:TitanRocketOffset2.bhvr
		Part	:RocketGlow1.part
		Part	:RocketFire1.part
		Part	:Rocketsmoke.part
		Geom	Malta_Rocket_Yellow
	End

End

Condition
	On	Prime7Hit
	Time	0

	Event
		Type	Local
		At	Origin
		#Sound	Effigy5 100 90 .75
	End
End


Condition
	On 	Prime7Hit
	Time 	0

	Event
		EName	Explosion7
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
	On 	Prime7Hit
	Time 	5

	Event
		EName	Smoke
		Type	Start
		At	T_Chest
		Part	:Smoke2.part
		Lifespan 5
	End

	Event
		Ename	Prime7
		Type	Destroy
	End

	Event
		Ename	Explosion7
		Type	Destroy
	End

End

Condition
	On	Time
	Time	42
	Event
		EName 	Prime8
		Type	start
		At	WepR
		Bhvr	:TitanRocket2.bhvr
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	Offset15
		Type	local
		At	Prime8
		Bhvr	:TitanRocketOffset3.bhvr
		Part	:RocketGlow1.part
		Part	:RocketFire1.part
		Part	:Rocketsmoke.part
		Geom	Malta_Rocket_Yellow
	End

	Event
		EName 	RocketFlash1
		Type	local
		At	Offset15
		Bhvr	:TitanRocketOffset.bhvr
		Part	:RocketFlash1.part
		Lifespan 5
	End

	Event
		EName 	Offset16
		Type	local
		At	Offset15
		Bhvr	:TitanRocketOffset2.bhvr
		Part	:RocketGlow1.part
		Part	:RocketFire1.part
		Part	:Rocketsmoke.part
		Geom	Malta_Rocket_Yellow
	End

End

Condition
	On	Prime8Hit
	Time	0

	Event
		Type	Local
		At	Origin
		#Sound	Effigy5 100 90 .75
	End
End


Condition
	On 	Prime8Hit
	Time 	0

	Event
		EName	Explosion8
		Type	Start
		At	T_Chest
		Part	:WhiteCenter.part
		Part	:Flash01.part
		Part	:Smoke1.part
		bhvr	:RocketExploShake2.bhvr
		Lifespan 5		
	End

	Event
		Ename	Prime8
		Type	Destroy
	End

	Event
		Ename	Explosion8
		Type	Destroy
	End
End

Condition
	On 	Prime8Hit
	Time 	5

	Event
		EName	Smoke
		Type	Start
		At	T_Chest
		Part	:Smoke2.part
		Lifespan 5
	End

End

End				