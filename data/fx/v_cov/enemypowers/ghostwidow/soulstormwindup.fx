#########################################################
##	Soul Storm
##
FxInfo


LifeSpan  100


################  Start   #####################



################   1   ##########################

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

		Lifespan	45
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
		
		Lifespan	45
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

		Lifespan	45
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
		
		Lifespan	45
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

		Lifespan	45
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
		
		Lifespan	45
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

############  Explode   ################


Condition
	On	Time
	Time	60

	Event
		EName 	All 
		Type	Destroy
	End

End

End				