#########################################################
##	Mirror Cage
########################################################
FxInfo

############################################

Condition
	On	Time	
	Time 	0

	Event
		Type 	Local
		At	Origin
		Sound Magicshield4_loop 69.0 69.0 .55
		#bhvr	behaviors/soundFade3.bhvr
		#Flags	PowerLoopingSound
		#Lifespan 420
	End

End

Condition
	On	Time	
	Time 	0

	Event
		Type 	Local
		At	Origin
		Sound Magicmud 69.0 69.0 .4
	
	End

End



##########  Heat Burst  ###############

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Ename	Heat0
#		Type	Start
#		At	hips
#
#		Geom	FX_BronzeMirrorBall
#
#		Bhvr	:DistortionBlast.bhvr
#
#		Lifespan	6
#	End
#
#End

#Condition
#	On 	Time
#	Time 	0
#
#	Repeat	25
#
#	Event
#		Ename	Heat3
#		Type	Start
#		At	Root
#
#		Geom	FX_BlurBall
#
#		Bhvr	:DistortionFade.bhvr
#
#		Lifespan	60
#		LifespanJitter	15
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	4
#
#	Repeat	25
#
#	Event
#		Ename	Heat1
#		Type	Start
#		At	Root
#
#		Geom	FX_BlurBall
#
#		Bhvr	:DistortionFade.bhvr
#		BhvrOverride
#			Scale			.75 .75 .75
#		End
#
#		Lifespan	60
#		LifespanJitter	15
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	2
#
#	Repeat	25
#
#	Event
#		Ename	Heat2
#		Type	Start
#		At	Root
#
#		Geom	FX_BlurBall
#
#		Bhvr	:DistortionFade.bhvr
#		BhvrOverride
#			Scale			.5 .5 .5
#		End
#
#		Lifespan	60
#		LifespanJitter	15
#	End
#
#End
#
#############  Continuing Heat  ##################



Condition
	On 	Cycle
	Time 	2
	Chance	.5


	Event
		Ename	Heat3
		Type	Local
		At	Base

		Geom	FX_BlurBall

		Bhvr	:DistortionFade.bhvr

		Lifespan	75
		LifespanJitter	30
	End

End

Condition
	On 	Cycle
	Time 	2
	Chance	.7


	Event
		Ename	Heat1
		Type	Local
		At	Base

		Geom	FX_BlurBall

		Bhvr	:DistortionFade.bhvr
		BhvrOverride
			Scale			.75 .75 .75
		End

		Lifespan	75
		LifespanJitter	30
	End

End

Condition
	On 	Cycle
	Time 	2
	Chance	.6


	Event
		Ename	Heat2
		Type	Local
		At	Base

		Geom	FX_BlurBall

		Bhvr	:DistortionFade.bhvr
		BhvrOverride
			Scale			.5 .5 .5
		End

		Lifespan	75
		LifespanJitter	30
	End

End

#
#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Ename	heatt
#		Type	Local
#		At	Root
#		ChildFX :HeatDistortion_SM.fx
#
#	End
#
##	Event	
##		Ename	embers
##		Type	Local
##		At	Root
##		part1	:HeatSpots.part
##		PMagnet Head
##	End
##
#	Event
#		Ename	gloww
#		Type	Start
#		At	Root
#		part	:holdheatglow.part
#		part2	:HoldEmberLocal.part
#	End
#
#End
#
############  Torso   ###################

#Condition
#	On 	Time
#	Time 	20
#
#	Repeat	4
#
#	Event
#		Ename	1
#		Type	Local
#		At	Chest
#		
#		bhvr	:MirrorCage.bhvr
#		BhvrOverride
#			PositionOffset		0 .1 0
#			StartJitter		.5 .6 .4
#			pyrRotateJitter		180 180 180
#
#		End
#		
#		#Geom	FX_BronzeMirrorShard_01
#		Geom	FX_BronzeMirrorShard_02
#		#Geom	FX_BronzeMirrorShard_03
#		Geom	FX_BronzeMirrorShard_04
#
##		Geom	FX_MoltenBronzeGlob_01
##		Geom	FX_MoltenBronzeGlob_02
#
#	End
#
#End
#
Condition
	On 	Time
	Time 	10

	Repeat	1

#	Event
#		Ename	2
#		Type	Local
#		At	Hips
#		
#		bhvr	:MirrorCage.bhvr
#		BhvrOverride
#			PositionOffset		0 0 0
#			StartJitter		.4 .4 .3
#			pyrRotateJitter		180 180 180
#		End
#			
#		Geom	FX_BronzeMirrorShard_01
#		Geom	FX_BronzeMirrorShard_02
#		Geom	FX_BronzeMirrorShard_03
#		Geom	FX_BronzeMirrorShard_04
#
##		Geom	FX_MoltenBronzeGlob_01
##		Geom	FX_MoltenBronzeGlob_02
#
#	End
#
##	Event
##		Ename	Sparks
##		Type	Start
##		At	Hips
##
##		Bhvr	V_COV\PhysicsEnabled\SparkUP.bhvr
##		BhvrOverride
##			InitialVelocity		.0 0.1 .0
##			InitialVelocityJitter	0.2 0.2 0.2
##		End
##
##		
##		Part	V_COV\PhysicsEnabled\MetalSpark.part
##		Part	V_COV\PhysicsEnabled\MetalSparkStreak.part
##
##		Lifespan	60
##		LifespanJitter	10
##	End

#	Event
#		EName	Sparkout
#		Type 	Start
#		At	Hips
#		Bhvr	V_COV\PhysicsEnabled\SparkUP.bhvr
#		BhvrOverride
#			InitialVelocity		.0 0.1 .0
#			InitialVelocityJitter	0.2 0.2 0.2
#
#			physRestitution 	0.5
#			physSFriction 	 	0.75
#			physDFriction 	 	0.75
#		End
#
#		Part	V_COV\PhysicsEnabled\ShrapnelSpark.part
#		Part	V_COV\PhysicsEnabled\HotSpark.part
#		Part	V_COV\PhysicsEnabled\HotSparkStreak.part
#
#		Lifespan	60
#		LifespanJitter	10
#		
#	End
#
End


Condition
	On 	Time
	Time 	15

	Repeat	2

#	Event
#		Ename	3
#		Type	Local
#		At	Waist
#		
#		bhvr	:MirrorCage.bhvr
#		BhvrOverride
#			PositionOffset		0 0 0
#			StartJitter		.3 .3 .3
#			pyrRotateJitter		180 180 180
#		End
#			
#		Geom	FX_BronzeMirrorShard_01
#		Geom	FX_BronzeMirrorShard_02
#		Geom	FX_BronzeMirrorShard_03
#		Geom	FX_BronzeMirrorShard_04
#
##		Geom	FX_MoltenBronzeGlob_01
##		Geom	FX_MoltenBronzeGlob_02
#
#	End
#
	Event
		Ename	Chunks
		Type	Start
		At	Head

		Geom	FX_MoltenBronzeGlob_02
		Geom	FX_BronzeMirrorGlob_02
		Geom	FX_MoltenBronzeGlob_01

		bhvr	:MoltenBronzeSplash.bhvr
		BhvrOverride
			Scale			.7 .7 .7
			pyrRotateJitter		180 180 180
			SpinJitter		.5 .5 .5
			InitialVelocity		.0 0.1 .0
			InitialVelocityJitter	0.2 0.2 0.2
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			Gravity			.04
			physRestitution 	0.3
			physSFriction 	 	0.3
			physDFriction 	 	0.3
			FadeOutLength		1110
		End

		CEvent 	:MoltenBronzeSplash_SM.fx
		CThresh 0.00001
		CDestroy	1

		Lifespan	300
		LifespanJitter	60
	End

End

#############  Upper  Arms   ################

Condition
	On 	Time
	Time 	15

	Repeat	2

#	Event
#		Ename	4
#		Type	Local
#		At	UarmR
#		
#		bhvr	:MirrorCage.bhvr
#		BhvrOverride
#			PositionOffset		.3 0 0
#			StartJitter		.3 .1 .1
#			pyrRotateJitter		180 180 180
#		End
#		
#		Geom	FX_BronzeGlob_01
#		Geom	FX_BronzeGlob_02
#
#	End
#
#	Event
#		EName	Sparkout
#		Type 	Start
#		At	UarmR
#		Bhvr	V_COV\PhysicsEnabled\SparkUP.bhvr
#		BhvrOverride
#			InitialVelocity		.0 0.1 .0
#			InitialVelocityJitter	0.2 0.2 0.2
#
#			physRestitution 	0.5
#			physSFriction 	 	0.75
#			physDFriction 	 	0.75
#		End
#
#		Part	V_COV\PhysicsEnabled\ShrapnelSpark.part
#		Part	V_COV\PhysicsEnabled\HotSpark.part
#		Part	V_COV\PhysicsEnabled\HotSparkStreak.part
#
#		Lifespan	60
#		LifespanJitter	10
#		
#	End
#
##	Event
##		Ename	Sparks
##		Type	Start
##		At	UarmR
##
##		Bhvr	V_COV\PhysicsEnabled\SparkUP.bhvr
##		BhvrOverride
##			InitialVelocity		.0 0.1 .0
##			InitialVelocityJitter	0.2 0.2 0.2
##		End
##
##		
##		Part	V_COV\PhysicsEnabled\MetalSpark.part
##		Part	V_COV\PhysicsEnabled\MetalSparkStreak.part
##
##		Lifespan	60
##		LifespanJitter	10
##	End

End


Condition
	On 	Time
	Time 	15

	Repeat	2

#	Event
#		Ename	5
#		Type	Local
#		At	UarmL
#		
#		bhvr	:MirrorCage.bhvr
#		BhvrOverride
#			PositionOffset		-.3 0 0
#			StartJitter		.3 .1 .1
#			pyrRotateJitter		180 180 180
#		End
#		
#		Geom	FX_BronzeGlob_01
#		Geom	FX_BronzeGlob_02
#
#	End
#
#	Event
#		EName	Sparkout
#		Type 	Start
#		At	UarmL
#		Bhvr	V_COV\PhysicsEnabled\SparkUP.bhvr
#		BhvrOverride
#			InitialVelocity		.0 0.1 .0
#			InitialVelocityJitter	0.2 0.2 0.2
#
#			physRestitution 	0.5
#			physSFriction 	 	0.75
#			physDFriction 	 	0.75
#		End
#
#		Part	V_COV\PhysicsEnabled\ShrapnelSpark.part
#		Part	V_COV\PhysicsEnabled\HotSpark.part
#		Part	V_COV\PhysicsEnabled\HotSparkStreak.part
#
#		Lifespan	60
#		LifespanJitter	10
#		
#	End
#
##	Event
##		Ename	Sparks
##		Type	Start
##		At	UarmL
##
##		Bhvr	V_COV\PhysicsEnabled\SparkUP.bhvr
##		BhvrOverride
##			InitialVelocity		.0 0.1 .0
##			InitialVelocityJitter	0.2 0.2 0.2
##		End
##
##		
##		Part	V_COV\PhysicsEnabled\MetalSpark.part
##		Part	V_COV\PhysicsEnabled\MetalSparkStreak.part
##
##		Lifespan	60
##		LifespanJitter	10
##	End
##
End

#############  Lower  Arms   ################

Condition
	On 	Time
	Time 	25

	Repeat	3

#	Event
#		Ename	6
#		Type	Local
#		At	LarmR
#		
#		bhvr	:MirrorCage.bhvr
#		BhvrOverride
#			PositionOffset		.3 0 0
#			StartJitter		.3 .1 .1
#			pyrRotateJitter		180 180 180
#		End
#		
#		Geom	FX_BronzeGlob_01
#		Geom	FX_BronzeGlob_02
#
#	End
#
#	Event
#		EName	Sparkout
#		Type 	Start
#		At	LarmR
#		Bhvr	V_COV\PhysicsEnabled\SparkUP.bhvr
#		BhvrOverride
#			InitialVelocity		.0 0.1 .0
#			InitialVelocityJitter	0.2 0.2 0.2
#
#			physRestitution 	0.5
#			physSFriction 	 	0.75
#			physDFriction 	 	0.75
#		End
#
#		Part	V_COV\PhysicsEnabled\ShrapnelSpark.part
#		Part	V_COV\PhysicsEnabled\HotSpark.part
#		Part	V_COV\PhysicsEnabled\HotSparkStreak.part
#
#		Lifespan	60
#		LifespanJitter	10
#		
#	End
#
##	Event
##		Ename	Sparks
##		Type	Start
##		At	LarmR
##
##		Bhvr	V_COV\PhysicsEnabled\SparkUP.bhvr
##		BhvrOverride
##			InitialVelocity		.0 0.1 .0
##			InitialVelocityJitter	0.2 0.2 0.2
##		End
##
##		
##		Part	V_COV\PhysicsEnabled\MetalSpark.part
##		Part	V_COV\PhysicsEnabled\MetalSparkStreak.part
##
##		Lifespan	60
##		LifespanJitter	10
##	End
##
End


Condition
	On 	Time
	Time 	25

	Repeat	3

#	Event
#		Ename	7
#		Type	Local
#		At	LarmL
#		
#		bhvr	:MirrorCage.bhvr
#		BhvrOverride
#			PositionOffset		-.3 0 0
#			StartJitter		.3 .1 .1
#			pyrRotateJitter		180 180 180
#		End
#		
#		Geom	FX_BronzeGlob_01
#		Geom	FX_BronzeGlob_02
#
#	End
#
#	Event
#		EName	Sparkout
#		Type 	Start
#		At	UarmL
#		Bhvr	V_COV\PhysicsEnabled\SparkUP.bhvr
#		BhvrOverride
#			InitialVelocity		.0 0.1 .0
#			InitialVelocityJitter	0.2 0.2 0.2
#
#			physRestitution 	0.5
#			physSFriction 	 	0.75
#			physDFriction 	 	0.75
#		End
#
#		Part	V_COV\PhysicsEnabled\ShrapnelSpark.part
#		Part	V_COV\PhysicsEnabled\HotSpark.part
#		Part	V_COV\PhysicsEnabled\HotSparkStreak.part
#
#		Lifespan	60
#		LifespanJitter	10
#		
#	End
#
##	Event
##		Ename	Sparks
##		Type	Start
##		At	LarmL
##
##		Bhvr	V_COV\PhysicsEnabled\SparkUP.bhvr
##		BhvrOverride
##			InitialVelocity		.0 0.1 .0
##			InitialVelocityJitter	0.2 0.2 0.2
##		End
##
##		
##		Part	V_COV\PhysicsEnabled\MetalSpark.part
##		Part	V_COV\PhysicsEnabled\MetalSparkStreak.part
##
##		Lifespan	60
##		LifespanJitter	10
##	End
##
End

############  Base   #################

Condition
	On 	Time
	Time 	0

	Event
		Ename	base
		Type	Local
		At	Root
		
		BhvrOverride
			PositionOffset		0 0 -.5
		End
		

	End

	Event
		Ename	basemetal
		Type	Local
		At	Base
		
		bhvr	:MirrorCageFade.bhvr
		BhvrOverride
			StartJitter		0 0 0
			pyrRotateJitter		0 180 0
		End
		
		Geom	FX_MoltenBronzeMass

	End

#	Event
#		Ename	pulse
#		Type	SetLight
#		At	basemetal
#		
#		bhvr	:MirrorCagePulse.bhvr
#
#		Lifespan	75
#	
#	End

End

####################  Fingers  #####################

#Condition
#	On 	Time
#	Time 	2
#
#	Event
#		Ename	Fingers
#		Type	Local
#		At	Root
#		
#		bhvr	:MirrorCageFingerNode.bhvr
#
#
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	2
#
#	Repeat	11
#
#	Event
#		Ename	Finger
#		Type	Local
#		At	Fingers
#		
#		bhvr	:MirrorCageFinger.bhvr
#		BhvrOverride
#			PositionOffset		0 3.25 0
#		End
#		
#		Geom	FX_MoltenBronzeFinger
#		Geom	FX_MoltenBronzeFinger01
#		Geom	FX_MoltenBronzeFinger02
#		Geom	FX_MoltenBronzeFinger03
#		Geom	FX_MoltenBronzeFinger04
#
#	End
#
#End
#
######  ring   ###################

Condition
	On 	Time
	Time 	5

	Event
		Ename	glob01
		Type	Start
		At	Base
		BhvrOverride
			pyrRotateJitter		0 180 0
		End
		
		
	End

	Event
		Ename	blob01
		Type	Local
		At	glob01
		
		bhvr	:MoltenBronzePuddle.bhvr
		BhvrOverride
			PositionOffset		0 0 2.0
			StartJitter		.2 0 .2
			Scale			2.5 .25 2.5
			pyrRotateJitter		180 180 180

		End
		
		Geom	FX_MoltenBronzeGlob_02
		Geom	FX_MoltenBronzeGlob_01
		Geom	FX_MoltenBronzeBall

	End

	Event
		Ename	glob02
		Type	Start
		At	Base
		BhvrOverride
			pyrRotateJitter		0 180 0
		End
		
		
	End

	Event
		Ename	blob02
		Type	Local
		At	glob02
		
		bhvr	:MoltenBronzePuddle.bhvr
		BhvrOverride
			PositionOffset		0 0 2.0
			StartJitter		.2 0 .2
			Scale			2.5 .25 2.5
			pyrRotateJitter		180 180 180

		End
		
		Geom	FX_MoltenBronzeGlob_02
		Geom	FX_MoltenBronzeGlob_01
		Geom	FX_MoltenBronzeBall

	End

	Event
		Ename	glob03
		Type	Start
		At	Base
		BhvrOverride
			pyrRotateJitter		0 180 0
		End
		
		
	End

	Event
		Ename	blob03
		Type	Local
		At	glob03
		
		bhvr	:MoltenBronzePuddle.bhvr
		BhvrOverride
			PositionOffset		0 0 2.0
			StartJitter		.2 0 .2
			Scale			2.5 .25 2.5
			pyrRotateJitter		180 180 180

		End
		
		Geom	FX_MoltenBronzeGlob_02
		Geom	FX_MoltenBronzeGlob_01
		Geom	FX_MoltenBronzeBall

	End

	Event
		Ename	glob04
		Type	Start
		At	Base
		BhvrOverride
			pyrRotateJitter		0 180 0
		End
		
		
	End

	Event
		Ename	blob04
		Type	Local
		At	glob04
		
		bhvr	:MoltenBronzePuddle.bhvr
		BhvrOverride
			PositionOffset		0 0 2.0
			StartJitter		.2 0 .2
			Scale			2.5 .25 2.5
			pyrRotateJitter		180 180 180

		End
		
		Geom	FX_MoltenBronzeGlob_02
		Geom	FX_MoltenBronzeGlob_01
		Geom	FX_MoltenBronzeBall

	End

	Event
		Ename	glob05
		Type	Start
		At	Base
		BhvrOverride
			pyrRotateJitter		0 180 0
		End
		
		
	End

	Event
		Ename	blob05
		Type	Local
		At	glob05
		
		bhvr	:MoltenBronzePuddle.bhvr
		BhvrOverride
			PositionOffset		0 0 2.0
			StartJitter		.2 0 .2
			Scale			2.5 .25 2.5
			pyrRotateJitter		180 180 180

		End
		
		Geom	FX_MoltenBronzeGlob_02
		Geom	FX_MoltenBronzeGlob_01
		Geom	FX_MoltenBronzeBall

	End

	Event
		Ename	glob06
		Type	Start
		At	Base
		BhvrOverride
			pyrRotateJitter		0 180 0
		End
		
		
	End

	Event
		Ename	blob06
		Type	Local
		At	glob06
		
		bhvr	:MoltenBronzePuddle.bhvr
		BhvrOverride
			PositionOffset		0 0 2.0
			StartJitter		.2 0 .2
			Scale			2.5 .25 2.5
			pyrRotateJitter		180 180 180

		End
		
		Geom	FX_MoltenBronzeGlob_02
		Geom	FX_MoltenBronzeGlob_01
		Geom	FX_MoltenBronzeBall

	End

	Event
		Ename	glob07
		Type	Start
		At	Base
		BhvrOverride
			pyrRotateJitter		0 180 0
		End
		
		
	End

	Event
		Ename	blob07
		Type	Local
		At	glob07
		
		bhvr	:MoltenBronzePuddle.bhvr
		BhvrOverride
			PositionOffset		0 0 2.0
			StartJitter		.2 0 .2
			Scale			2.5 .25 2.5
			pyrRotateJitter		180 180 180

		End
		
		Geom	FX_MoltenBronzeGlob_02
		Geom	FX_MoltenBronzeGlob_01
		Geom	FX_MoltenBronzeBall

	End

	Event
		Ename	glob08
		Type	Start
		At	Base
		BhvrOverride
			pyrRotateJitter		0 180 0
		End
		
		
	End

	Event
		Ename	blob08
		Type	Local
		At	glob08
		
		bhvr	:MoltenBronzePuddle.bhvr
		BhvrOverride
			PositionOffset		0 0 2.0
			StartJitter		.2 0 .2
			Scale			2.5 .25 2.5
			pyrRotateJitter		180 180 180

		End
		
		Geom	FX_MoltenBronzeGlob_02
		Geom	FX_MoltenBronzeGlob_01
		Geom	FX_MoltenBronzeBall

	End

	Event
		Ename	glob09
		Type	Start
		At	Base
		BhvrOverride
			pyrRotateJitter		0 180 0
		End
		
		
	End

	Event
		Ename	blob09
		Type	Local
		At	glob09
		
		bhvr	:MoltenBronzePuddle.bhvr
		BhvrOverride
			PositionOffset		0 0 2.0
			StartJitter		.2 0 .2
			Scale			2.5 .25 2.5
			pyrRotateJitter		180 180 180

		End
		
		Geom	FX_MoltenBronzeGlob_02
		Geom	FX_MoltenBronzeGlob_01
		Geom	FX_MoltenBronzeBall

	End

	Event
		Ename	glob010
		Type	Start
		At	Base
		BhvrOverride
			pyrRotateJitter		0 180 0
		End
		
		
	End

	Event
		Ename	blob010
		Type	Local
		At	glob010
		
		bhvr	:MoltenBronzePuddle.bhvr
		BhvrOverride
			PositionOffset		0 0 2.0
			StartJitter		.2 0 .2
			Scale			2.5 .25 2.5
			pyrRotateJitter		180 180 180

		End
		
		Geom	FX_MoltenBronzeGlob_02
		Geom	FX_MoltenBronzeGlob_01
		Geom	FX_MoltenBronzeBall

	End

	Event
		Ename	glob011
		Type	Start
		At	Base
		BhvrOverride
			pyrRotateJitter		0 180 0
		End
		
		
	End

	Event
		Ename	blob011
		Type	Local
		At	glob011
		
		bhvr	:MoltenBronzePuddle.bhvr
		BhvrOverride
			PositionOffset		0 0 2.0
			StartJitter		.2 0 .2
			Scale			2.5 .25 2.5
			pyrRotateJitter		180 180 180

		End
		
		Geom	FX_MoltenBronzeGlob_02
		Geom	FX_MoltenBronzeGlob_01
		Geom	FX_MoltenBronzeBall

	End

	Event
		Ename	glob012
		Type	Start
		At	Base
		BhvrOverride
			pyrRotateJitter		0 180 0
		End
		
		
	End

	Event
		Ename	blob012
		Type	Local
		At	glob012
		
		bhvr	:MoltenBronzePuddle.bhvr
		BhvrOverride
			PositionOffset		0 0 2.0
			StartJitter		.2 0 .2
			Scale			2.5 .25 2.5
			pyrRotateJitter		180 180 180

		End
		
		Geom	FX_MoltenBronzeGlob_02
		Geom	FX_MoltenBronzeGlob_01
		Geom	FX_MoltenBronzeBall

	End

	Event
		Ename	glob013
		Type	Start
		At	Base
		BhvrOverride
			pyrRotateJitter		0 180 0
		End
		
		
	End

	Event
		Ename	blob013
		Type	Local
		At	glob013
		
		bhvr	:MoltenBronzePuddle.bhvr
		BhvrOverride
			PositionOffset		0 0 1.8
			StartJitter		.2 0 .2
			Scale			2.5 .25 2.5
			pyrRotateJitter		180 180 180

		End
		
		Geom	FX_MoltenBronzeGlob_02
		Geom	FX_MoltenBronzeGlob_01
		Geom	FX_MoltenBronzeBall

	End

End


########  Explosion   ####################

Condition
	On 	Time
	Time 	7

	Repeat	15
	RepeatJitter	3

	Event
		Ename	Chunks
		Type	Start
		At	Base

		Geom	FX_MoltenBronzeGlob_02
		Geom	FX_MoltenBronzeGlob_01

		bhvr	:MoltenBronzeSplash.bhvr
		BhvrOverride
			PositionOffset		0 1 0
			StartJitter		2 0 2
			Scale			.7 .7 .7
			pyrRotateJitter		180 180 180
			SpinJitter		.5 .5 .5
			InitialVelocity		.0 0.5 .0
			InitialVelocityJitter	0.25 0.3 0.25
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			Gravity			.04
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
		End
		CEvent 	:MoltenBronzeSplash.fx
		CThresh 0.0005
		CDestroy	1

		Lifespan	300
		LifespanJitter	60
	End

End

##Condition
##	On 	Time
##	Time 	15
##		
##	Repeat	15
##	RepeatJitter	2
##	
##	Event
##		EName	Sparkout
##		Type 	Start
##		At	root
##		Bhvr	V_COV\PhysicsEnabled\SparkUP.bhvr
##		BhvrOverride
##			PositionOffset		0 1 0
##			StartJitter		2 0 2
##
##			physRestitution 	0.5
##			physSFriction 	 	0.75
##			physDFriction 	 	0.75
##		End
##
##		CThresh 	0.000001
##
##		Part	V_COV\PhysicsEnabled\MetalSpark.part
##		Part	V_COV\PhysicsEnabled\MetalSparkStreak.part
##
###		Part	V_COV\PhysicsEnabled\ShrapnelSpark.part
###		Part	V_COV\PhysicsEnabled\HotSpark.part
###		Part	V_COV\PhysicsEnabled\HotSparkStreak.part
##
##		Lifespan	60
##		LifespanJitter	10
##		
##	End
##
##End
##
############  Continuing  Sparks  #################


##Condition
##	On 	Time
##	Time 	2
##	Repeat	30
##
##	Event
##		Ename	Sparks
##		Type	Start
##		At	root
##
##		Bhvr	V_COV\PhysicsEnabled\SparkUP.bhvr
##		BhvrOverride
##			PositionOffset		0 1.5 0
##			StartJitter		1.5 .5 1.5
##
##			InitialVelocity		.0 0.4 .0
##			InitialVelocityJitter	0.25 0.25 0.25
##		End
##
##		
##		Part	V_COV\PhysicsEnabled\MetalSpark.part
##		Part	V_COV\PhysicsEnabled\MetalSparkStreak.part
##
##		Lifespan	60
##		LifespanJitter	10
##	End
##
##End
##
#Condition
#	On 	Time
#	Time 	30
#		
#	Repeat	8
#	RepeatJitter	2
#	
#	Event
#		EName	Sparkout
#		Type 	Start
#		At	root
#		Bhvr	V_COV\PhysicsEnabled\SparkUP.bhvr
#		BhvrOverride
#			PositionOffset		0 1 0
#			StartJitter		2 0 2
#
#			physRestitution 	0.5
#			physSFriction 	 	0.75
#			physDFriction 	 	0.75
#		End
#
#		CThresh 	0.000001
#
#		Part	V_COV\PhysicsEnabled\ShrapnelSpark.part
#		Part	V_COV\PhysicsEnabled\HotSpark.part
#		Part	V_COV\PhysicsEnabled\HotSparkStreak.part
#
#		Lifespan	60
#		LifespanJitter	10
#		
#	End
#
#End
#

End
