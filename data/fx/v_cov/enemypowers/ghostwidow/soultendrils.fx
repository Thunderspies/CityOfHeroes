#############################################################
## SoulTendrils.fx
#############################################################

FxInfo

LifeSpan  120

#############################################################

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	origin
		Sound widowcast1 80 80 1.0
	End
End

Condition
	On	Time
	Time	15
	Event
		EName 	binder7
		Type	local
		At	UarmR
		Bhvr	:soulBinderArm2.bhvr
		Lifespan	20
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

Condition
	On	Time
	Time	15

	Event
		EName 	binder8
		Type	local
		At	UarmL
		Bhvr	:soulBinderArm1.bhvr
		Lifespan	20
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

################   3   ##########################

Condition
	On	Time
	Time	23
	Event
		EName 	binder9
		Type	local
		At	larmR
		Bhvr	:soulBinderArm2.bhvr
		Lifespan	20
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

################   4   ##########################

Condition
	On	Time
	Time	23

	Event
		EName 	binder10
		Type	local
		At	larmL
		Bhvr	:soulBinderArm1.bhvr
		Lifespan	20
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

#############################################################

Condition
	On	Time
	Time	30

	Event
		EName 	HandR
		Type	local
		At	WepR
		Part	:soulGlow1.part
		Part	:soulFire2.part
		Part	:soulSmoke1.part

		Lifespan	23
	End

	Event
		EName 	HandL
		Type	local
		At	WepL
		Part	:soulGlow1.part
		Part	:soulFire2.part
		Part	:soulSmoke1.part

		Lifespan	23
	End
End

#############################################################

Condition
	On Time
	Time 20

	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	:soulLights.bhvr
		LifeSpan 20
	End
End

#############################################################

Condition
	On	Time
	Time	30

	Event
		EName 	Prime
		Type	start
		At	WepL
		Bhvr	:soulProjectile1.bhvr
		Magnet	Target
		LookAt	Target
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

#############################################################

Condition
	On	Time
	Time	33

	Event
		EName 	Prime2
		Type	start
		At	WepR
		Bhvr	:soulProjectile2.bhvr
		Magnet	Target
		LookAt	Target
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

#############################################################

Condition
	On	Time
	Time	36

	Event
		EName 	Prime3
		Type	start
		At	WepL
		Bhvr	:soulProjectile1.bhvr
		Magnet	Target
		LookAt	Target
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

#############################################################

Condition
	On	Time
	Time	39
	Event
		EName 	Prime4
		Type	start
		At	WepR
		Bhvr	:soulProjectile2.bhvr
		Magnet	Target
		LookAt	Target
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

#############################################################

Condition
	On	PrimeHit

	Event
		EName 	Explode
		Type	Start
		At	T_Root
		Part	:soulFlash2.part
		Part	:soulExplode2.part
		Part	:soulSparks1.part
		Part	:soulCloud1.part
		Part	POWERS\DarknessControl\DarkBlastPuddleExplosion.Part
		Sound dirtexplo 70 70 .77
		Lifespan 5
	End

	Event
		EName 	Prime
		Type	Destroy
	End
End

Condition
	On	Prime1Hit

	Event
		EName 	Prime1
		Type	Destroy
	End
End

Condition
	On	Prime2Hit

	Event
		EName 	Prime2
		Type	Destroy
	End
End

Condition
	On	Prime3Hit

	Event
		EName 	Prime3
		Type	Destroy
	End
End

Condition
	On	Prime4Hit

	Event
		EName 	Prime4
		Type	Destroy
	End
End


#############################################################

End