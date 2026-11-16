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
	#	Sound	Ice7 100 80 .6
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
	#	part1   :IceSparkles.part
	#	Sound	Ice1_loop 100 85 .45
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
	#	ChildFX	:BodyFrost.fx
		Lifespan	37
	End


#############################################################
##################    STONE GEO   ###########################
#############################################################



	Event
		EName 	Icesickle12
		Type	PositOnly
		At	Root
	#	Bhvr 	behaviors/powers/coldcontrol/DeepFreeze02.bhvr
		BhvrOverride
			Scale 		1.5 1.5 1.5
			Alpha 		255
			PyrRotate 	0 180 0
			FadeinLength	0
			FadeOutLength	0
		End
	#	Part	:GroundSmoke_Alpha.part
	#	Part	:GroundSmoke_Additive.part
	#	geom	FX_StoneWall_01
		Lifespan 60
	End

	Event
		EName 	Icesickle12
		Type	PositOnly
		At	Root
	#	Bhvr 	behaviors/powers/coldcontrol/DeepFreeze02.bhvr
		BhvrOverride
			Scale 		1.5 1.0 1.0
			Alpha 		255
			PyrRotate 	0 180 0
			FadeinLength	0
			FadeOutLength	0
		End
	#	Part	:GroundSmoke_Alpha.part
	#	Part	:GroundSmoke_Additive.part
		geom	FX_StoneWall_05
		Lifespan 60
	End

	Event
		EName 	Icesickle12
		Type	PositOnly
		At	Root
	#	Bhvr 	behaviors/powers/coldcontrol/DeepFreeze02.bhvr
		BhvrOverride
			Scale 		1.0 1.0 1.0
			Alpha 		255
		#	PyrRotate 	0 180 0
			FadeinLength	0
			FadeOutLength	0
		End
	#	Part	:GroundSmoke_Alpha.part
	#	Part	:GroundSmoke_Additive.part
		geom	FX_StoneWall_07
		Lifespan 60
	End
End

#############################################################

#Condition
#	On 	Time
#	Time 	7
#
#	Event
#		EName 	Icesickle04
#		Type	PositOnly
#		At	Root
#		Bhvr	behaviors/powers/coldcontrol/DeepFreeze02.bhvr
#		BhvrOverride
#			PyrRotate 0 60 0
#		End
#		geom	FX_IceWall_04
#		Lifespan 20
#
#	End
#End
#
##############################################################
#
#
#Condition
#	On 	Time
#	Time 	9
#
#	Event
#		EName 	Icesickle09
#		Type	PositOnly
#		At	Root
#		Bhvr	behaviors/powers/coldcontrol/DeepFreeze02.bhvr
#		BhvrOverride
#			PyrRotate -10 0 0
#			EndScale 1 3 1
#		End
#		geom	FX_IceWall_09
#		Lifespan 20
#
#	End
#End


#############################################################
##################    EXPLOSION  ############################
#############################################################

Condition
	On	Time
	Time 	43

	Event
		Type	Local
		At	Hips
	#	Part	:Lightbeams.part
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
	#	Part	:BlastFlash.part
	#	Part	:LargeBlast.part
	#	Part	:BlastRing.part
	#	Part	:Sparks.part

		Lifespan 70
		LifespanJitter 20
	End


#############################################################
##################   PHYSICS  ###############################
#############################################################

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


#############################################################
##################   END   ##################################
#############################################################