#########################################################
##	Soul Storm
##
FxInfo


LifeSpan  300


################   1   ##########################

Condition
	On 	Time
	Time 	0
	
	Event
		Type	Local
		At 	origin
		#Sound	Flamebolt4 80 30 .4
		Sound Soulstorm2 80 80 .66
	End
End


Condition
	On 	Time
	Time 	41
	
	Event
		Type	Local
		At 	origin
		Sound soulstorm1 80 80 .9
	End
End

Condition
	On	Time
	Time	0
	Event
		EName 	binder1 
		Type	local
		At	footR
		Bhvr	:soulBinder1.bhvr

		Lifespan	45
	End

	Event
		EName 	Offset1 
		Type	local
		At	binder1
		Bhvr	:binderOffset.bhvr
		Part	:soulGlow0.part
		Part	:soulFire0.part
		Part	:soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1 
		Type	local
		At	Offset1
		  AltPiv	2
		Part	:soulFire0.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset1
		  AltPiv	4
		Part	:soulFire0.part
	End

End


################   2   ##########################

Condition
	On	Time
	Time	0
	Event
		EName 	binder2 
		Type	local
		At	footL
		Bhvr	:soulBinder2.bhvr
		
		Lifespan	45
	End

	Event
		EName 	Offset2 
		Type	local
		At	binder2
		Bhvr	:binderOffset.bhvr
		Part	:soulGlow0.part
		Part	:soulFire0.part
		Part	:soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1 
		Type	local
		At	Offset2
		  AltPiv	2
		Part	:soulFire0.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset2
		  AltPiv	4
		Part	:soulFire0.part
	End

End


################   3   ##########################

Condition
	On	Time
	Time	10
	Event
		EName 	binder3 
		Type	local
		At	llegR
		Bhvr	:soulBinder1.bhvr

		Lifespan	40
	End

	Event
		EName 	Offset3 
		Type	local
		At	binder3
		Bhvr	:binderOffset.bhvr
		Part	:soulGlow0.part
		Part	:soulFire0.part
		Part	:soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1 
		Type	local
		At	Offset3
		  AltPiv	2
		Part	:soulFire0.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset3
		  AltPiv	4
		Part	:soulFire0.part
	End

End


################   4   ##########################

Condition
	On	Time
	Time	10
	Event
		EName 	binder4 
		Type	local
		At	llegL
		Bhvr	:soulBinder2.bhvr
		
		Lifespan	40
	End

	Event
		EName 	Offset4 
		Type	local
		At	binder4
		Bhvr	:binderOffset.bhvr
		Part	:soulGlow0.part
		Part	:soulFire0.part
		Part	:soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1 
		Type	local
		At	Offset4
		  AltPiv	2
		Part	:soulFire0.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset4
		  AltPiv	4
		Part	:soulFire0.part
	End

End


################   5   ##########################

Condition
	On	Time
	Time	15
	Event
		EName 	binder5 
		Type	local
		At	Hips
		Bhvr	:soulBinder1.bhvr

		Lifespan	35
	End

	Event
		EName 	Offset5 
		Type	local
		At	binder5
		Bhvr	:binderOffset.bhvr
		Part	:soulGlow0.part
		Part	:soulFire0.part
		Part	:soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1 
		Type	local
		At	Offset5
		  AltPiv	2
		Part	:soulFire0.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset5
		  AltPiv	4
		Part	:soulFire0.part
	End

End


################   6   ##########################

Condition
	On	Time
	Time	20
	Event
		EName 	binder6 
		Type	local
		At	Waist
		Bhvr	:soulBinder2.bhvr
		
		Lifespan	25
	End

	Event
		EName 	Offset6 
		Type	local
		At	binder6
		Bhvr	:binderOffset.bhvr
		Part	:soulGlow0.part
		Part	:soulFire0.part
		Part	:soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1 
		Type	local
		At	Offset6
		  AltPiv	2
		Part	:soulFire0.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset6
		  AltPiv	4
		Part	:soulFire0.part
	End

End

################   7   ##########################

Condition
	On	Time
	Time	5
	Event
		EName 	binder7 
		Type	local
		At	handR
		Bhvr	:soulBinderArm1.bhvr

		Lifespan	45
	End

	Event
		EName 	Offset7 
		Type	local
		At	binder7
		Bhvr	:binderOffset.bhvr
		Part	:soulGlow0.part
		Part	:soulFire0.part
		Part	:soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1 
		Type	local
		At	Offset7
		  AltPiv	2
		Part	:soulFire0.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset7
		  AltPiv	4
		Part	:soulFire0.part
	End

End


################   8   ##########################

Condition
	On	Time
	Time	5

	Event
		EName 	binder8 
		Type	local
		At	handL
		Bhvr	:soulBinderArm2.bhvr
		
		Lifespan	45
	End

	Event
		EName 	Offset8 
		Type	local
		At	binder8
		Bhvr	:binderOffset.bhvr
		Part	:soulGlow0.part
		Part	:soulFire0.part
		Part	:soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1 
		Type	local
		At	Offset8
		  AltPiv	2
		Part	:soulFire0.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset8
		  AltPiv	4
		Part	:soulFire0.part
	End

End

################   9   ##########################

Condition
	On	Time
	Time	15
	Event
		EName 	binder9 
		Type	local
		At	larmR
		Bhvr	:soulBinderArm1.bhvr

		Lifespan	35
	End

	Event
		EName 	Offset9 
		Type	local
		At	binder9
		Bhvr	:binderOffset.bhvr
		Part	:soulGlow0.part
		Part	:soulFire0.part
		Part	:soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1 
		Type	local
		At	Offset9
		  AltPiv	2
		Part	:soulFire0.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset9
		  AltPiv	4
		Part	:soulFire0.part
	End

End


################   10   ##########################

Condition
	On	Time
	Time	15

	Event
		EName 	binder10 
		Type	local
		At	larmL
		Bhvr	:soulBinderArm2.bhvr
		
		Lifespan	35
	End

	Event
		EName 	Offset10 
		Type	local
		At	binder10
		Bhvr	:binderOffset.bhvr
		Part	:soulGlow0.part
		Part	:soulFire0.part
		Part	:soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1 
		Type	local
		At	Offset10
		  AltPiv	2
		Part	:soulFire0.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset10
		  AltPiv	4
		Part	:soulFire0.part
	End

End

################  Launch   #####################


Condition
	On Time
	Time 45


	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	:soulLights.bhvr
		LifeSpan 20
	End


	Event
		EName 	spirit1
		Type	local
		At	Target
		Bhvr	:soulOrbit.bhvr
		Geom	helixFXoffset
	End


	Event
		EName 	orbit1 
		Type	local
		At	spirit1
		  Altpiv	1
		Bhvr	:soulOrbit.bhvr
	End
	Event
		EName 	spirit2
		Type	local
		At	Target
		Bhvr	:soulOrbit.bhvr
		Geom	helixFXoffset
	End


	Event
		EName 	orbit2 
		Type	local
		At	spirit2
		  Altpiv	1
		Bhvr	:soulOrbit.bhvr
	End

	Event
		EName 	spirit3
		Type	local
		At	Target
		Bhvr	:soulOrbit.bhvr
		Geom	helixFXoffset
	End


	Event
		EName 	orbit3 
		Type	local
		At	spirit3
		  Altpiv	1
		Bhvr	:soulOrbit.bhvr
	End

	Event
		EName 	spirit4
		Type	local
		At	Target
		Bhvr	:soulOrbit.bhvr
		Geom	helixFXoffset
	End


	Event
		EName 	orbit4 
		Type	local
		At	spirit4
		  Altpiv	1
		Bhvr	:soulOrbit.bhvr
	End

	Event
		EName 	spirit5
		Type	local
		At	Target
		Bhvr	:soulOrbit.bhvr
		Geom	helixFXoffset
	End


	Event
		EName 	orbit5 
		Type	local
		At	spirit5
		  Altpiv	1
		Bhvr	:soulOrbit.bhvr
	End

	Event
		EName 	spirit6
		Type	local
		At	Target
		Bhvr	:soulOrbit.bhvr
		Geom	helixFXoffset
	End


	Event
		EName 	orbit6 
		Type	local
		At	spirit6
		  Altpiv	1
		Bhvr	:soulOrbit.bhvr
	End

End


################   P1   ##########################

Condition
	On	Time
	Time	45
	Event
		EName 	Prime
		Type	start
		At	WepL
		Bhvr	:soulProjectile1.bhvr
		Magnet	orbit1
		LookAt	orbit1
		Lifespan	150
	End

	Event
		EName 	Offset1 
		Type	local
		At	Prime
		Bhvr	:soulOffset.bhvr
		Part	:soulGlow1.part
		Part	:soulFire2.part
		Part	:soulSmoke1.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1 
		Type	local
		At	Offset1
		  AltPiv	2
		Part	:soulFire1.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset1
		  AltPiv	4
		Part	:soulFire1.part
	End


End


#######################  P2  ####################

Condition
	On	Time
	Time	48
	Event
		EName 	Prime2 
		Type	start
		At	WepR
		Bhvr	:soulProjectile2.bhvr
		Magnet	orbit2
		LookAt	orbit2
		Lifespan	150
	End

	Event
		EName 	Offset2 
		Type	local
		At	Prime2
		Bhvr	:soulOffset3.bhvr
		Part	:soulGlow1.part
		Part	:soulFire2.part
		Part	:soulSmoke1.part
		Geom	stormFXsoul
	End


	Event
		EName 	Arm1 
		Type	local
		At	Offset2
		  AltPiv	2
		Part	:soulFire1.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset2
		  AltPiv	4
		Part	:soulFire1.part
	End


End


######################## P3 ############################


Condition
	On	Time
	Time	51
	Event
		EName 	Prime3 
		Type	start
		At	WepL
		Bhvr	:soulProjectile1.bhvr
		Magnet	orbit3
		LookAt	orbit3
		Lifespan	150
	End

	Event
		EName 	Offset3 
		Type	local
		At	Prime3
		Bhvr	:soulOffset3.bhvr
		Part	:soulGlow1.part
		Part	:soulFire2.part
		Part	:soulSmoke1.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1 
		Type	local
		At	Offset3
		  AltPiv	2
		Part	:soulFire1.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset3
		  AltPiv	4
		Part	:soulFire1.part
	End

End

#######################   P4   ################################

Condition
	On	Time
	Time	54
	Event
		EName 	Prime4 
		Type	start
		At	WepR
		Bhvr	:soulProjectile2.bhvr
		Magnet	orbit4
		LookAt	orbit4
		Lifespan	150
	End

	Event
		EName 	Offset4 
		Type	local
		At	Prime4
		Bhvr	:soulOffset.bhvr
		Part	:soulGlow1.part
		Part	:soulFire2.part
		Part	:soulSmoke1.part
		Geom	stormFXsoul
	End


	Event
		EName 	Arm1 
		Type	local
		At	Offset4
		  AltPiv	2
		Part	:soulFire1.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset4
		  AltPiv	4
		Part	:soulFire1.part
	End

End

################   P5   ##########################

Condition
	On	Time
	Time	57
	Event
		EName 	Prime5 
		Type	start
		At	WepL
		Bhvr	:soulProjectile1.bhvr
		Magnet	orbit5
		LookAt	orbit5
		Lifespan	150
	End

	Event
		EName 	Offset5 
		Type	local
		At	Prime5
		Bhvr	:soulOffset.bhvr
		Part	:soulGlow1.part
		Part	:soulFire2.part
		Part	:soulSmoke1.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1 
		Type	local
		At	Offset5
		  AltPiv	2
		Part	:soulFire1.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset5
		  AltPiv	4
		Part	:soulFire1.part
	End


End


#######################  P6  ####################

Condition
	On	Time
	Time	60
	Event
		EName 	Prime6 
		Type	start
		At	WepR
		Bhvr	:soulProjectile2.bhvr
		Magnet	orbit6
		LookAt	orbit6
		Lifespan	150
	End

	Event
		EName 	Offset6 
		Type	local
		At	Prime6
		Bhvr	:soulOffset3.bhvr
		Part	:soulGlow1.part
		Part	:soulFire2.part
		Part	:soulSmoke1.part
		Geom	stormFXsoul
	End


	Event
		EName 	Arm1 
		Type	local
		At	Offset6
		  AltPiv	2
		Part	:soulFire1.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset6
		  AltPiv	4
		Part	:soulFire1.part
	End

End

#############   TimeOut    ################

Condition
	On	Prime4Hit

	Event
		EName 	Prime1
		Type	start
		At	Chest
		Bhvr	:soulProjectile2.bhvr
		Magnet	Target
		LookAt	Target
		Lifespan	150
	End
End


Condition
	On	Prime1Hit

	Event
		EName 	All 
		Type	Destroy
		
	End
End

End				