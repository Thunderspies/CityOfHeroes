#############################################################
## Moonbeam_Windup.fx
#############################################################

FxInfo

#########################################################

Condition
	On 	Time
	Time 	15

	Event
		EName 	Hand1
		Type	Local
		At	UarmR
		Part1	:DarkSoulBeamTenticle.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb.part
		Sound widowdarkwindup 80 80 .7
		PMagnet	LarmR
	End


	Event
		EName 	Hand2
		Type	Local
		At	LarmR
		Part1	:DarkSoulBeamTenticle.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb.part
		PMagnet	WepR
	End

End

################  Start   #####################

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


Condition
	On	Time
	Time	30
	Event
		EName 	binder1
		Type	local
		At	handR
		Bhvr	:soulBinderArm2.bhvr
		BhvrOverride
			InitialVelocity 0 0 0
			InitialVelocityJitter	0 0 0
			SpinJitter	0.05 0.015 0.015
		End

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

Condition
	On	Time
	Time	30

	Event
		EName 	HandR
		Type	local
		At	WepR
		Part	:soulGlow1.part
		Part	:soulSmoke0.part

	End


End

#############################################################

End


