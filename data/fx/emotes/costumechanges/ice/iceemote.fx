#############################################################
## Cond Control - Chillblain
#############################################################

FxInfo

Lifespan 120
#############################################################
##################    AUDIO   ###############################
#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	root
		Sound Ice7 100 100 .6
	End
End

#############################################################

Condition
	On 	Time
	Time 	3

	Event
		EName 	Prime
		Type	posit
		At	Root
		part1   :IceSparkles.part
		Sound Ice1_loop 100 100 .45
		Lifespan 	30
		LifespanJitter	10
	End
End


#############################################################
##################    BODY FROST   ##########################
#############################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	posit
		At	Root
		ChildFX	:BodyFrost.fx
		Lifespan	37
	End


#############################################################
##################    ICE GEO   #############################
#############################################################



	Event
		EName 	Icesickle12
		Type	PositOnly
		At	Root
		Bhvr 	behaviors/BlockOfIce.bhvr
		Part	:DeepFreezeIceChunks.part
		geom	Hybernate
		Lifespan 40
	End
End

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		EName 	Icesickle04
		Type	PositOnly
		At	Root
		Bhvr	behaviors/powers/coldcontrol/DeepFreeze02.bhvr
		BhvrOverride
			PyrRotate 0 60 0
		End
		geom	FX_IceWall_04
		Lifespan 20

	End
End

#############################################################


Condition
	On 	Time
	Time 	9

	Event
		EName 	Icesickle09
		Type	PositOnly
		At	Root
		Bhvr	behaviors/powers/coldcontrol/DeepFreeze02.bhvr
		BhvrOverride
			PyrRotate -10 0 0
			EndScale 1 3 1
		End
		geom	FX_IceWall_09
		Lifespan 20

	End
End


#############################################################
##################    EXPLOSION  ############################
#############################################################

Condition
	On	Time
	Time 	43

	Event
		Type	Local
		At	Hips
		Part	:Lightbeams.part
		Lifespan 15
		LifespanJitter 5
	End
End

#############################################################

Condition
	On	Time
	Time	45




	Event
		Type	Local
		At	Hips
		Part	:BlastFlash.part
		Part	:LargeBlast.part
		Part	:BlastRing.part
		Part	:Sparks.part

		Lifespan 70
		LifespanJitter 20
	End

End


#############################################################

Condition
	On	Time
	Time	48
		Event
		Type	Local
		At	Hips
		Part	:BreakoutCrystal01.part
		Part	:BreakoutCrystal02.part
		Part	:Smoke.part
		Part	:SmokeLight.part
		Part	:SmokeLight2.part
		Lifespan 70
		LifespanJitter 20
	End

End

#############################################################

Condition
	On	Time
	Time	50
	Repeat 	7

	Event
		Type	Local
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\IceChunkScatter02.fx
		Lifespan 70
		LifespanJitter 20
	End
End

#############################################################
##################   PHYSICS  ###############################
#############################################################

Condition

	On	Time
	Time 	43

	Event
		Ename	PHYSICS-ExplosiveForce
		Type	Start
		At	Chest

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		600
			PhysForcePowerJitter	75
		End
		Lifespan	1

	End
End

#############################################################
##################   END   ##################################
#############################################################