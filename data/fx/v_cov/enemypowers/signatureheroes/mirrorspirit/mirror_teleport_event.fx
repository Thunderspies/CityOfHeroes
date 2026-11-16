#########################################################
##	Mirror Cage
########################################################


FxInfo
Lifespan 60

########  offset for mirror spawn  #################

Condition
	On	Time
	Time	0

	Event
		EName	Offset
		Type	Start
		At	Root
##		Geom	Testing_Target
				Bhvr	Behaviors/GenericParticleFade.bhvr
                        BhvrOverride
                                    PositionOffset		0 0 0   ##-.4 .2 -1 ##.4 .7 -.7
			End
		Lifespan	50
		
	End

End

####  Test   ########################################



Condition
	On 	time
	Time 	1

	Event
		Ename	test
		Type	Local
		At	Offset  ##Root
		Anim	FX_mirror_teleport
		Bhvr	:MirrorTeleportFade.bhvr
		Lifespan	50
	End

End

#########  Boil Up ###########


Condition
	On 	time
	Time 	0

	Event
		Ename	radius
		Type	Local
		At	Offset
		Bhvr	:MirrorBubbleIN.bhvr
		Lifespan	300
	End

	Event
		Ename	base
		Type	Local
		At	Offset

		#Geom	FX_BlurBall

		#Bhvr	:TeleportDistortion.bhvr
		BhvrOverride
			PositionOffset	0 .3 0
#			StartJitter	0 0 0
#			SpinJitter		0.1 0.0 0.1
#			InitialVelocity		0.0 0.0 0.0
#			InitialVelocityJitter	0.0 0.0 0.0
#			Gravity		0
#			Scale		10 1 10
		End
		
		Part	:HoldEmberLocal.part

		Lifespan	45
	End

End

Condition
	On 	Time
	Time 	1

	Repeat	7

	Event
		Ename	boils
		Type	Local
		At	radius
		ChildFX :MirrorTeleportBubble.fx
		Lifespan	300
	End

End

Condition
	On 	Cycle
	Time 	1

	Repeat	4

	Event
		Ename	boils
		Type	Local
		At	radius
		ChildFX :MirrorTeleportBubble.fx
		Lifespan	300
	End

End

#########  Boil Down  ###########


Condition
	On 	Time
	Time 	45

	Repeat	25

	Event
		Ename	boils2
		Type	Local
		At	Offset
		ChildFX :MirrorTeleportBubble2.fx
		Lifespan	30
	End

End

Condition
	On 	Time
	Time 	48

	Repeat	25

	Event
		Ename	boils2
		Type	Local
		At	Offset
		ChildFX :MirrorTeleportBubble2.fx
		Lifespan	30
	End

End

Condition
	On 	Time
	Time 	51

	Repeat	25

	Event
		Ename	boils2
		Type	Local
		At	Offset
		ChildFX :MirrorTeleportBubble2.fx
		Lifespan	30
	End

End


########  Explosion   ####################

Condition
	On 	Time
	Time 	7

	Repeat	12
	RepeatJitter	3

	Event
		Ename	Chunks
		Type	Start
		At	Offset

		Geom	FX_MoltenBronzeGlob_02
		Geom	FX_MoltenBronzeGlob_01

		bhvr	:MoltenBronzeSplash.bhvr
		BhvrOverride
			PositionOffset		0 1 0
			StartJitter		2 0 2
			Scale			.7 .7 .7
			pyrRotateJitter		180 180 180
			SpinJitter		.5 .5 .5
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.2 0.3 0.2
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
		End
		CEvent 	:MirrorTeleportSplash.fx
		CThresh 0.0005
		CDestroy	1

		Lifespan	120
		LifespanJitter	60
	End

End

Condition
	On 	time
	Time 	15

	Repeat	9
	RepeatJitter	3

	Event
		Ename	Chunks
		Type	Start
		At	Offset

		Geom	FX_MoltenBronzeGlob_02
		Geom	FX_BronzeMirrorGlob_02
		Geom	FX_MoltenBronzeGlob_01

		bhvr	:MoltenBronzeSplash.bhvr
		BhvrOverride
			PositionOffset		0 5 0
			Scale			.7 .7 .7
			pyrRotateJitter		180 180 180
			SpinJitter		.5 .5 .5
			InitialVelocity		.0 0.4 .0
			InitialVelocityJitter	0.1 0.2 0.1
			StartJitter		1.5 1 1
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.3
			physSFriction 	 	0.3
			physDFriction 	 	0.3
			FadeOutLength		1110
		End

		CEvent 	:MirrorTeleportSplash_SM.fx
		CThresh 0.00001
		CDestroy	1

		Lifespan	120
		LifespanJitter	60
	End

End

######  drippy test  #########################################################################


Condition
	On 	Cycle
	Time 	1
	Chance	.8

	Repeat	1

	Event
		Ename	Heat4
		Type	Local
		At	Offset

		Geom	FX_BlurBall

		Bhvr	:TeleportDistortion.bhvr

		Lifespan	30
		LifespanJitter	15
	End

End

Condition
	On 	Cycle
	Time 	1
	Chance	.6

	Repeat	1

	Event
		Ename	Heat5
		Type	Local
		At	Offset

		Geom	FX_BlurBall

		Bhvr	:TeleportDistortion.bhvr
		BhvrOverride
			Scale			.75 .75 .75
		End

		Lifespan	30
		LifespanJitter	15
	End

End

Condition
	On 	Cycle
	Time 	1
	Chance	.7

	Repeat	1

	Event
		Ename	Heat6
		Type	Local
		At	Offset

		Geom	FX_BlurBall

		Bhvr	:TeleportDistortion.bhvr
		BhvrOverride
			Scale			.5 .5 .5
		End

		Lifespan	30
		LifespanJitter	15
	End

End


End
