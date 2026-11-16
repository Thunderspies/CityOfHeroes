#########################################################
##	Soul Storm
##
FxInfo


LifeSpan  120


################  Start   #####################

Condition
	On	Time
	Time	10
	Event
		EName 	binder7
		Type	local
		At	UarmR
		Bhvr	:soulBinderArm2.bhvr
		Sound summonwidow2 80 80 .86
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


################   2   ##########################

Condition
	On	Time
	Time	10

	Event
		EName 	binder8
		Type	local
		At	UarmL
		Bhvr	:soulBinderArm1.bhvr
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

################   3   ##########################

Condition
	On	Time
	Time	20
	Event
		EName 	binder9
		Type	local
		At	larmR
		Bhvr	:soulBinderArm2.bhvr

		Lifespan	60
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
	Time	20

	Event
		EName 	binder10
		Type	local
		At	larmL
		Bhvr	:soulBinderArm1.bhvr

		Lifespan	60
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

############   Hands   ###########


Condition
	On	Time
	Time	10

	Event
		EName 	HandR
		Type	local
		At	WepR
		Part	:soulGlow1.part
		Part	:soulFire2.part
		Part	:soulSmoke1.part
	End


	Event
		EName 	HandL
		Type	local
		At	WepL
		Part	:soulGlow1.part
		Part	:soulFire2.part
		Part	:soulSmoke1.part
	End

End

############  Explode   ################

Condition
	On	Time
	Time	100

	Event
		EName 	All
		Type	Destroy
	End

End

End