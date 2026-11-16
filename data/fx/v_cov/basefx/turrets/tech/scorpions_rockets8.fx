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

LifeSpan  100

######################################################################################################
########################################  Emitter Barrel  ############################################
######################################################################################################

Condition
	On	Time
	Time	0

	Event	
		EName	Emitter
		Type	Local
		At	WepR
		BhvrOverride
			behavior
			positionOffset		0 0 0
		End

#		Geom	Testing_TargetB
	End
End

#######################################################
######################## 1 ############################
#######################################################

#Condition
#	On	Time
#	Time	12
#	Event
#		Type	Local
#		At	Rwing2
#		Sound	Servodoor4 200 160 .79
#	End
#End


Condition
	On	Time
	Time	20
	Event
		Type	Local
		At	Rwing2
		Sound rocketlaunch10 200 200 .89
	End
End

Condition
	On	Time
	Time	31
	Event
		Type	Local
		At	Rwing2
		#Sound	rocketlaunch7 200 160 .76
	End
End


Condition
	On	Time
	Time	30
	Event
		Type	Local
		At	Rwing2
		#Sound	rocketlaunch 200 160 .79
	End
End


Condition
	On	Time
	Time	52
	Event
		Type	Local
		At	Rwing2
		#Sound	rocketlaunch9 200 160 .75
	End
End


Condition
	On	Time
	Time	68
	Event
		Type	Local
		At	Rwing2
		#Sound	rocketlaunch4 200 60 .85
	End
End



Condition
	On Time
	Time 20
	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	ENEMYFX\Malta\RocketFX\RocketLights.bhvr
		LifeSpan 5
	End

End

Condition
	On Time
	Time 26
	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	ENEMYFX\Malta\RocketFX\RocketLights.bhvr
		LifeSpan 5
	End

End

Condition
	On Time
	Time 32
	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	ENEMYFX\Malta\RocketFX\RocketLights.bhvr
		LifeSpan 5
	End

End

Condition
	On Time
	Time 38
	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	ENEMYFX\Malta\RocketFX\RocketLights.bhvr
		LifeSpan 5
	End

End

Condition
	On Time
	Time 44
	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	ENEMYFX\Malta\RocketFX\RocketLights.bhvr
		LifeSpan 5
	End

End

Condition
	On Time
	Time 50
	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	ENEMYFX\Malta\RocketFX\RocketLights.bhvr
		LifeSpan 5
	End

End

Condition
	On Time
	Time 56
	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	ENEMYFX\Malta\RocketFX\RocketLights.bhvr
		LifeSpan 5
	End

End

Condition
	On Time
	Time 62
	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	ENEMYFX\Malta\RocketFX\RocketLights.bhvr
		LifeSpan 5
	End

End



Condition
	On	Time
	Time	20
	Event
		EName 	Prime 
		Type	start
		At	Emitter
		Bhvr	V_COV\BaseFx\turrets\Tech\Scorpion_RocketTravel.bhvr
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	Offset1 
		Type	local
		At	Prime
#		Bhvr	ENEMYFX\Malta\RocketFX\TitanRocketOffset.bhvr
		Part	ENEMYFX\Malta\RocketFX\RocketGlow1Kronos.part
		Part	ENEMYFX\Malta\RocketFX\RocketFire1Kronos.part
		Part	ENEMYFX\Malta\RocketFX\RocketsmokeKronos.part
#		Geom	Malta_Rocket_Orange
	End

	Event
		EName	GeomOffset01
		Type	Local
		At	Prime
		BhvrOverride
			behavior
			positionOffset		0 0 1.5
		End
		Geom	Malta_Rocket_Orange
	End

	Event
		EName 	RocketFlash1
		Type	local
		At	Offset1
		Bhvr	ENEMYFX\Malta\RocketFX\TitanRocketOffset.bhvr
		Part	V_COV\BaseFx\turrets\Tech\Scorpion_RocketFlash1.part
		Lifespan 5
	End

#	Event
#		EName 	Offset2 
#		Type	local
#		At	Offset1
#		Bhvr	ENEMYFX\Malta\RocketFX\TitanRocketOffset2.bhvr
#		Part	ENEMYFX\Malta\RocketFX\RocketGlow1Kronos.part
#		Part	ENEMYFX\Malta\RocketFX\RocketFire1Kronos.part
#		Part	ENEMYFX\Malta\RocketFX\RocketsmokeKronos.part
#		Geom	Malta_Rocket_Orange
#	End

End

Condition
	On	PrimeHit

	Event
		Type	Local
		At	target
		Sound explohit2 100 100 .85
	End
End


Condition
	On 	PrimeHit

	Event
		EName	Explosion1
		Type	Start
		At	T_Chest
		Part	ENEMYFX\Malta\RocketFX\WhiteCenter.part
		Part	ENEMYFX\Malta\RocketFX\Flash01.part
		Part	ENEMYFX\Malta\RocketFX\Smoke1.part
		bhvr	ENEMYFX\Malta\RocketFX\RocketExploShake2.bhvr
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
		Part	ENEMYFX\Malta\RocketFX\Smoke2.part
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

########################################################
######################### 2 ############################
########################################################

Condition
	On	Time
	Time	26
	Event
		EName 	Prime2 
		Type	start
		At	Emitter
		Bhvr	V_COV\BaseFx\turrets\Tech\Scorpion_RocketTravel.bhvr
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	Offset3 
		Type	local
		At	Prime2
		Bhvr	V_COV\BaseFx\turrets\Tech\Scorpion_RocketOffset3.bhvr
		Part	ENEMYFX\Malta\RocketFX\RocketGlow1Kronos.part
		Part	ENEMYFX\Malta\RocketFX\RocketFire1Kronos.part
		Part	ENEMYFX\Malta\RocketFX\RocketsmokeKronos.part
#		Geom	Malta_Rocket_Orange
	End

	Event
		EName	GeomOffset02
		Type	Local
		At	Prime2
		BhvrOverride
			behavior
			positionOffset		0 0.5 1.5
		End
		Geom	Malta_Rocket_Orange
	End

	Event
		EName 	RocketFlash1
		Type	local
		At	Emitter
		Bhvr	ENEMYFX\Malta\RocketFX\TitanRocketOffset.bhvr
		Part	V_COV\BaseFx\turrets\Tech\Scorpion_RocketFlash1.part
		Lifespan 5
	End

#	Event
#		EName 	Offset4 
#		Type	local
#		At	Offset3
#		Bhvr	ENEMYFX\Malta\RocketFX\TitanRocketOffset2.bhvr
#		Part	ENEMYFX\Malta\RocketFX\RocketGlow1Kronos.part
#		Part	ENEMYFX\Malta\RocketFX\RocketFire1Kronos.part
#		Part	ENEMYFX\Malta\RocketFX\RocketsmokeKronos.part
#		Geom	Malta_Rocket_Orange
#	End

End

Condition
	On	Prime2Hit
	Time	0

	Event
		Type	Local
		At	target
		Sound smokegrexplo 100 100 .85
	End
End


Condition
	On 	Prime2Hit
	Time 	0

	Event
		EName	Explosion2
		Type	Start
		At	T_Chest
		Part	ENEMYFX\Malta\RocketFX\WhiteCenter.part
		Part	ENEMYFX\Malta\RocketFX\Flash01.part
		Part	ENEMYFX\Malta\RocketFX\Smoke1.part
		bhvr	ENEMYFX\Malta\RocketFX\RocketExploShake2.bhvr
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
		Part	ENEMYFX\Malta\RocketFX\Smoke2.part
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

########################################################
######################### 3 ############################
########################################################

Condition
	On	Time
	Time	32
	Event
		EName 	Prime3 
		Type	start
		At	Emitter
		Bhvr	V_COV\BaseFx\turrets\Tech\Scorpion_RocketTravel.bhvr
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	Offset5 
		Type	local
		At	Prime3
#		Bhvr	ENEMYFX\Malta\RocketFX\TitanRocketOffset.bhvr
		Part	ENEMYFX\Malta\RocketFX\RocketGlow1Kronos.part
		Part	ENEMYFX\Malta\RocketFX\RocketFire1Kronos.part
		Part	ENEMYFX\Malta\RocketFX\RocketsmokeKronos.part
#		Geom	Malta_Rocket_Orange
	End

	Event
		EName	GeomOffset03
		Type	Local
		At	Prime3
		BhvrOverride
			behavior
			positionOffset		0 0 1.5
		End
		Geom	Malta_Rocket_Orange
	End

	Event
		EName 	RocketFlash1
		Type	local
		At	Emitter
#		Bhvr	ENEMYFX\Malta\RocketFX\TitanRocketOffset.bhvr
		Part	V_COV\BaseFx\turrets\Tech\Scorpion_RocketFlash1.part
		Lifespan 5
	End

#	Event
#		EName 	Offset6 
#		Type	local
#		At	Offset5
#		Bhvr	ENEMYFX\Malta\RocketFX\TitanRocketOffset2.bhvr
#		Part	ENEMYFX\Malta\RocketFX\RocketGlow1Kronos.part
#		Part	ENEMYFX\Malta\RocketFX\RocketFire1Kronos.part
#		Part	ENEMYFX\Malta\RocketFX\RocketsmokeKronos.part
#		Geom	Malta_Rocket_Orange
#	End

End

Condition
	On	Prime3Hit
	Time	0

	Event
		Type	Local
		At	target
		Sound grexplo4 100 100 .85
	End
End


Condition
	On 	Prime3Hit
	Time 	0

	Event
		EName	Explosion3
		Type	Start
		At	T_Chest
		Part	ENEMYFX\Malta\RocketFX\WhiteCenter.part
		Part	ENEMYFX\Malta\RocketFX\Flash01.part
		Part	ENEMYFX\Malta\RocketFX\Smoke1.part
		bhvr	ENEMYFX\Malta\RocketFX\RocketExploShake2.bhvr
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
		Part	ENEMYFX\Malta\RocketFX\Smoke2.part
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

########################################################
######################### 4 ############################
########################################################

Condition
	On	Time
	Time	38
	Event
		EName 	Prime4 
		Type	start
		At	Emitter
		Bhvr	V_COV\BaseFx\turrets\Tech\Scorpion_RocketTravel.bhvr
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	Offset7 
		Type	local
		At	Prime4
		Bhvr	V_COV\BaseFx\turrets\Tech\Scorpion_RocketOffset3.bhvr
		Part	ENEMYFX\Malta\RocketFX\RocketGlow1Kronos.part
		Part	ENEMYFX\Malta\RocketFX\RocketFire1Kronos.part
		Part	ENEMYFX\Malta\RocketFX\RocketsmokeKronos.part
#		Geom	Malta_Rocket_Orange
	End

	Event
		EName	GeomOffset04
		Type	Local
		At	Prime4
		BhvrOverride
			behavior
			positionOffset		0 .5 1.5
		End
		Geom	Malta_Rocket_Orange
	End
	Event
		EName 	RocketFlash1
		Type	local
		At	Emitter
#		Bhvr	ENEMYFX\Malta\RocketFX\TitanRocketOffset.bhvr
		Part	V_COV\BaseFx\turrets\Tech\Scorpion_RocketFlash1.part
		Lifespan 5
	End

#	Event
#		EName 	Offset8
#		Type	local
#		At	Offset7
#		Bhvr	ENEMYFX\Malta\RocketFX\TitanRocketOffset2.bhvr
#		Part	ENEMYFX\Malta\RocketFX\RocketGlow1Kronos.part
#		Part	ENEMYFX\Malta\RocketFX\RocketFire1Kronos.part
#		Part	ENEMYFX\Malta\RocketFX\RocketsmokeKronos.part
#		Geom	Malta_Rocket_Orange
#	End

End

Condition
	On	Prime4Hit
	Time	0

	Event
		Type	Local
		At	target
		Sound grexplo2 100 100 .85
	End
End


Condition
	On 	Prime4Hit
	Time 	0

	Event
		EName	Explosion4
		Type	Start
		At	T_Chest
		Part	ENEMYFX\Malta\RocketFX\WhiteCenter.part
		Part	ENEMYFX\Malta\RocketFX\Flash01.part
		Part	ENEMYFX\Malta\RocketFX\Smoke1.part
		bhvr	ENEMYFX\Malta\RocketFX\RocketExploShake2.bhvr
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
		Part	ENEMYFX\Malta\RocketFX\Smoke2.part
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

########################################################
######################### 5 ############################
########################################################

Condition
	On	Time
	Time	44
	Event
		EName 	Prime5 
		Type	start
		At	Emitter
		Bhvr	V_COV\BaseFx\turrets\Tech\Scorpion_RocketTravel.bhvr
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	Offset9 
		Type	local
		At	Prime5
#		Bhvr	ENEMYFX\Malta\RocketFX\TitanRocketOffset.bhvr
		Part	ENEMYFX\Malta\RocketFX\RocketGlow1Kronos.part
		Part	ENEMYFX\Malta\RocketFX\RocketFire1Kronos.part
		Part	ENEMYFX\Malta\RocketFX\RocketsmokeKronos.part
#		Geom	Malta_Rocket_Orange
	End

	Event
		EName	GeomOffset05
		Type	Local
		At	Prime5
		BhvrOverride
			behavior
			positionOffset		0 0 1.5
		End
		Geom	Malta_Rocket_Orange
	End

	Event
		EName 	RocketFlash1
		Type	local
		At	Emitter
#		Bhvr	ENEMYFX\Malta\RocketFX\TitanRocketOffset.bhvr
		Part	V_COV\BaseFx\turrets\Tech\Scorpion_RocketFlash1.part
		Lifespan 5
	End

#	Event
#		EName 	Offset10 
#		Type	local
#		At	Offset9
#		Bhvr	ENEMYFX\Malta\RocketFX\TitanRocketOffset2.bhvr
#		Part	ENEMYFX\Malta\RocketFX\RocketGlow1Kronos.part
#		Part	ENEMYFX\Malta\RocketFX\RocketFire1Kronos.part
#		Part	ENEMYFX\Malta\RocketFX\RocketsmokeKronos.part
#		Geom	Malta_Rocket_Orange
#	End

End

Condition
	On	Prime5Hit
	Time	0

	Event
		Type	Local
		At	target
		Sound flashgrenade 100 100 .75
	End
End


Condition
	On 	Prime5Hit
	Time 	0

	Event
		EName	Explosion5
		Type	Start
		At	T_Chest
		Part	ENEMYFX\Malta\RocketFX\WhiteCenter.part
		Part	ENEMYFX\Malta\RocketFX\Flash01.part
		Part	ENEMYFX\Malta\RocketFX\Smoke1.part
		bhvr	ENEMYFX\Malta\RocketFX\RocketExploShake2.bhvr
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
		Part	ENEMYFX\Malta\RocketFX\Smoke2.part
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

#######################################################
######################## 6 ############################
#######################################################

Condition
	On	Time
	Time	50
	Event
		EName 	Prime6
		Type	start
		At	Emitter
		Bhvr	V_COV\BaseFx\turrets\Tech\Scorpion_RocketTravel.bhvr
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	Offset11 
		Type	local
		At	Prime6
		Bhvr	V_COV\BaseFx\turrets\Tech\Scorpion_RocketOffset3.bhvr
		Part	ENEMYFX\Malta\RocketFX\RocketGlow1Kronos.part
		Part	ENEMYFX\Malta\RocketFX\RocketFire1Kronos.part
		Part	ENEMYFX\Malta\RocketFX\RocketsmokeKronos.part
#		Geom	Malta_Rocket_Orange
	End

	Event
		EName	GeomOffset06
		Type	Local
		At	Prime6
		BhvrOverride
			behavior
			positionOffset		0 .5 1.5
		End
		Geom	Malta_Rocket_Orange
	End

	Event
		EName 	RocketFlash1
		Type	local
		At	Emitter
#		Bhvr	ENEMYFX\Malta\RocketFX\TitanRocketOffset.bhvr
		Part	V_COV\BaseFx\turrets\Tech\Scorpion_RocketFlash1.part
		Lifespan 5
	End

#	Event
#		EName 	Offset12
#		Type	local
#		At	Offset11
#		Bhvr	ENEMYFX\Malta\RocketFX\TitanRocketOffset2.bhvr
#		Part	ENEMYFX\Malta\RocketFX\RocketGlow1Kronos.part
#		Part	ENEMYFX\Malta\RocketFX\RocketFire1Kronos.part
#		Part	ENEMYFX\Malta\RocketFX\RocketsmokeKronos.part
#		Geom	Malta_Rocket_Orange
#	End

End

Condition
	On	Prime6Hit
	Time	0

	Event
		Type	Local
		At	target
		Sound smokegrexplo 100 100 .75
	End
End


Condition
	On 	Prime6Hit
	Time 	0

	Event
		EName	Explosion6
		Type	Start
		At	T_Chest
		Part	ENEMYFX\Malta\RocketFX\WhiteCenter.part
		Part	ENEMYFX\Malta\RocketFX\Flash01.part
		Part	ENEMYFX\Malta\RocketFX\Smoke1.part
		bhvr	ENEMYFX\Malta\RocketFX\RocketExploShake2.bhvr
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
		Part	ENEMYFX\Malta\RocketFX\Smoke2.part
		Lifespan 5
	End

End


########################################################
######################### 7 ############################
########################################################

Condition
	On	Time
	Time	56
	Event
		EName 	Prime7 
		Type	start
		At	Emitter
		Bhvr	V_COV\BaseFx\turrets\Tech\Scorpion_RocketTravel.bhvr
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	Offset13
		Type	local
		At	Prime7
#		Bhvr	ENEMYFX\Malta\RocketFX\TitanRocketOffset.bhvr
		Part	ENEMYFX\Malta\RocketFX\RocketGlow1Kronos.part
		Part	ENEMYFX\Malta\RocketFX\RocketFire1Kronos.part
		Part	ENEMYFX\Malta\RocketFX\RocketsmokeKronos.part
#		Geom	Malta_Rocket_Orange
	End

	Event
		EName	GeomOffset07
		Type	Local
		At	Prime7
		BhvrOverride
			behavior
			positionOffset		0 0 1.5
		End
		Geom	Malta_Rocket_Orange
	End

	Event
		EName 	RocketFlash1
		Type	local
		At	Emitter
#		Bhvr	ENEMYFX\Malta\RocketFX\TitanRocketOffset.bhvr
		Part	V_COV\BaseFx\turrets\Tech\Scorpion_RocketFlash1.part
		Lifespan 5
	End

#	Event
#		EName 	Offset14 
#		Type	local
#		At	Offset13
#		Bhvr	ENEMYFX\Malta\RocketFX\TitanRocketOffset2.bhvr
#		Part	ENEMYFX\Malta\RocketFX\RocketGlow1Kronos.part
#		Part	ENEMYFX\Malta\RocketFX\RocketFire1Kronos.part
#		Part	ENEMYFX\Malta\RocketFX\RocketsmokeKronos.part
#		Geom	Malta_Rocket_Orange
#	End

End

Condition
	On	Prime7Hit
	Time	0

	Event
		Type	Local
		At	target
		Sound grexplo2 100 100 .65
	End
End


Condition
	On 	Prime7Hit
	Time 	0

	Event
		EName	Explosion7
		Type	Start
		At	T_Chest
		Part	ENEMYFX\Malta\RocketFX\WhiteCenter.part
		Part	ENEMYFX\Malta\RocketFX\Flash01.part
		Part	ENEMYFX\Malta\RocketFX\Smoke1.part
		bhvr	ENEMYFX\Malta\RocketFX\RocketExploShake2.bhvr
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
		Part	ENEMYFX\Malta\RocketFX\Smoke2.part
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

#######################################################
######################## 8 ############################
#######################################################

Condition
	On	Time
	Time	62
	Event
		EName 	Prime8
		Type	start
		At	Emitter
		Bhvr	V_COV\BaseFx\turrets\Tech\Scorpion_RocketTravel.bhvr
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	Offset15
		Type	local
		At	Prime8
		Bhvr	V_COV\BaseFx\turrets\Tech\Scorpion_RocketOffset3.bhvr
		Part	ENEMYFX\Malta\RocketFX\RocketGlow1Kronos.part
		Part	ENEMYFX\Malta\RocketFX\RocketFire1Kronos.part
		Part	ENEMYFX\Malta\RocketFX\RocketsmokeKronos.part
#		Geom	Malta_Rocket_Orange
	End

	Event
		EName	GeomOffset08
		Type	Local
		At	Prime8
		BhvrOverride
			behavior
			positionOffset		0 .5 1.5
		End
		Geom	Malta_Rocket_Orange
	End

	Event
		EName 	RocketFlash1
		Type	local
		At	Emitter
#		Bhvr	ENEMYFX\Malta\RocketFX\TitanRocketOffset.bhvr
		Part	V_COV\BaseFx\turrets\Tech\Scorpion_RocketFlash1.part
		Lifespan 5
	End

#	Event
#		EName 	Offset16
#		Type	local
#		At	Offset15
#		Bhvr	ENEMYFX\Malta\RocketFX\TitanRocketOffset2.bhvr
#		Part	ENEMYFX\Malta\RocketFX\RocketGlow1Kronos.part
#		Part	ENEMYFX\Malta\RocketFX\RocketFire1Kronos.part
#		Part	ENEMYFX\Malta\RocketFX\RocketsmokeKronos.part
#		Geom	Malta_Rocket_Orange
#	End

End

Condition
	On	Prime8Hit
	Time	0

	Event
		Type	Local
		At	target
		Sound smokegrexplo 100 100 .95
	End
End


Condition
	On 	Prime8Hit
	Time 	0

	Event
		EName	Explosion8
		Type	Start
		At	T_Chest
		Part	ENEMYFX\Malta\RocketFX\WhiteCenter.part
		Part	ENEMYFX\Malta\RocketFX\Flash01.part
		Part	ENEMYFX\Malta\RocketFX\Smoke1.part
		bhvr	ENEMYFX\Malta\RocketFX\RocketExploShake2.bhvr
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
		Part	ENEMYFX\Malta\RocketFX\Smoke2.part
		Lifespan 5
	End

End

End				