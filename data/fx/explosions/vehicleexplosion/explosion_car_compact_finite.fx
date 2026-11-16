#############################################################
## Expolsion_Car_Compact_Finite.fx
#############################################################

FxInfo
Lifespan 1200

#############################################################


Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime1
		Type	Local
		At	chest

		Sound car_explo1 80 80 .9

	End

End



########   Explosion Force   #########################

Condition
	On	Time
	Time	0

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Root

		BhvrOverride
			PositionOffset		0 2 0

			PhysForceType		Out
			PhysForceRadius		25
			PhysForcePower		800
			PhysForcePowerJitter	100
		End

		Lifespan	1

	End

End

########  Main Explosion Particles   #########################

Condition
	On 	Time
	Time 	0


	Event
		EName 	base
		Type	Create
		Inherit	Position Rotation
		Update	Position
		At	Root
		BhvrOverride
			PositionOffset	0 3 0
		End

	End



	Event
		EName 	Sound
		Type	Posit
		At	base
		Sound molotovhit1 80 80 .86
		#Sound	fireexplo 80 30 .72
		#Sound	firezap 80 30 .76
		#Sound	molotovhit1 80 30 .86
		Sound grexplo2 120.0 120.0 1.0

		LifeSpan	10

	End

	Event
		EName 	Fire
		Type	local
		At	base
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		ChildFX	med_fire.fx

		LifeSpan	20

	End

	Event
		EName 	Offset1
		Type	local
		At	base
		BhvrOverride
			PositionOffset	0 0 3
		End

		LifeSpan	25

	End

	Event
		EName 	Flames1
		Type	Local
		At	Offset1
		ChildFX	med_fire.fx

		LifeSpan	25

	End

	Event
		EName 	Offset2
		Type	local
		At	base
		BhvrOverride
			PositionOffset	0 0 -3
		End

		LifeSpan	25

	End

	Event
		EName 	Flames2
		Type	local
		At	Offset2
		ChildFX	med_fire.fx

		LifeSpan	25

	End

	Event
		EName 	Smoke
		Type	local
		At	base

		Part	:SmokeExplosion.part

		Lifespan 15
	End

	Event
		EName 	Explosion
		Type	local
		At	base

		Part1	:FireExplosion2.part
		Part2	:Inferno.part
		Part	:ExplosionlightStreams.part

		Lifespan 8
	End

	Event
		EName 	Explosion2
		Type	local
		At	base
		Part1	:FireExplosion.part
		Part2	:Inferno2.part

		Lifespan 12
	End


	Event
		EName 	flat
		Type	Local
		At	Root

		Part2	:FlatInferno.part
		Part	:FlatExplosionStar.part
		LifeSpan	10

	End

	Event
		EName 	Flash
		Type	local
		At	base

		Part	:FireExplosionFlash.part
		Part	:ExplosionStar.part

		Lifespan 5

	End

End

Condition
	On 	Time
	Time 	3

	Event
		EName 	chunks
		Type	Local
		At	base
		Part1	Generic\InfernorockBits.part
		Part2	Generic\InfernorockBitz.part
		Lifespan	10
	End

End

########   Generic Explosion Physics Particles   ###################

Condition
	On 	time
	Time 	0
	Repeat 	7

	Event
		EName 	Sparkz
		Type 	Start
		At 	base
		ChildFX V_COV\PhysicsEnabled\HotSparkOUT.fx
		Lifespan 60
	End

	Event
		EName 	Sparkz2
		HardwareOnly
		Type 	Start
		At 	base
		ChildFX V_COV\PhysicsEnabled\HotSparkUP.fx
		Lifespan 60
	End
End

Condition
	On 	time
	Time 	0
	Repeat 	15

	Event
		EName 	Sparkzz
		Type 	Start
		At 	base
		ChildFX V_COV\PhysicsEnabled\HotSparkExplode.fx
		Lifespan 60
	End
End


Condition
	On 	time
	Time 	0
	Repeat  7
	RepeatJitter	3

	Event
		EName 	napalm
		Type 	Local
		At 	base
		ChildFX V_COV\PhysicsEnabled\FireBurstScatter.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

Condition
	On	Time
	Time 	3
	Repeat	6

	Event
		EName 	metal
		Type	Start
		At	base
		Geom	Metal_Curl
		Geom	Metal_Flat

		Part3	:FireBallSpark.part

		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			FadeOutLength		130
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.2 0.5
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	1220
		LifespanJitter	30
	End

End

Condition
	On	Time
	Time 	3
	Repeat	9

	Event
		Ename	parts2
		HardwareOnly
		Type	Start
		At	base

		Part3	:FireBallSpark.part

		Part	V_COV\PhysicsEnabled\FireSplashBig.part

		Geom	carpart_sprocket
		Geom	carpart_pipe

		Bhvr	:PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 1 2

			PyrRotateJitter		180 180 180

			Scale			0.8 0.8 0.8

			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5

			InitialVelocity		0.0 0.5 0.0
			InitialVelocityJitter	1.2 0.8 1.2

			Drag			0.01

			physSFriction 	 	0.6

			PhysDebris		0

		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	1220
		LifespanJitter	30
	End

End

Condition
	On	Time
	Time 	3
	Repeat	8  #12

	Event
		Ename	parts3
		HardwareOnly
		Type	Start
		At	base

		Part3	:FireBallSpark.part
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part

		Part	V_COV\PhysicsEnabled\FireSplashBig.part

		Geom	Metal_Flat01
		Geom	Metal_Flat02
		Geom	Metal_Flat03
		Geom	Widget_01
		Geom	Widget_02
		Geom	Widget_03
		#Geom	Widget_04
		Geom	Widget_05
		Geom	Widget_06
		Geom	Widget_07
		Geom	Widget_08
		Geom	Widget_09
		Geom	Widget_10
		Geom	Widget_12
		Geom	Widget_13
		Geom	Widget_14

		Bhvr	:PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 1 2

			PyrRotateJitter		180 180 180

			Scale			.5 .5 .5

			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5

			InitialVelocity		0.0 0.5 0.0
			InitialVelocityJitter	1.2 0.8 1.2

			Drag			0.01

			physSFriction 	 	0.6

			PhysDebris		0

		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	60
		LifespanJitter	30
	End


End

###########################################################
########   Explosion Physics Objects   ###################
##########################################################

Condition
	Time 	0

#####################  Main Chassis  #########################

	Event
		Ename	Husk
		Type	Start
		At	base
		Part	:SmokeExplosion.part

		ChildFX	med_fire.fx

		Geom	carpart_mini_frame
		Bhvr	:PhysicsDebrisFade.bhvr
		BhvrOverride
			#Scale			1 1 1
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.12 0.12 0.12

			InitialVelocity		0.0 0.65 -0.0
			InitialVelocityJitter	0.15 0.2 0.15

			PhysDebris		0
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	1220
		LifespanJitter	30
	End

	Event
		EName 	EngineFire
		Type	Local
		At	Husk
		BhvrOverride
			PositionOffset	0 0 4
		End
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part

	End

##################### Tires  ######################

	Event
		Ename	TireRightRear
		Type	Start
		At	base

		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part


		Geom	carpart_wheel_small
		Bhvr	:PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		3 0 -4

			Scale			1 1 1
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5

			InitialVelocity		0.3 0.5 -0.3
			InitialVelocityJitter	0.2 0.3 0.4

			physRestitution 	0.7

			PhysDebris		0
		End
		CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
		CThresh 0.0001

		Lifespan	1220
		LifespanJitter	30
	End

	Event
		Ename	TireLeftRear
		Type	Start
		At	base

		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part


		Geom	carpart_wheel_small
		Bhvr	:PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-3 0 -4

			Scale			1 1 1
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5

			InitialVelocity		-0.3 0.5 -0.3
			InitialVelocityJitter	0.2 0.3 0.4

			physRestitution 	0.7

			PhysDebris		0
		End
		CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
		CThresh 0.0001

		Lifespan	1220
		LifespanJitter	30
	End

	Event
		Ename	TireRightFront
		Type	Start
		At	base

		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part


		Geom	carpart_wheel_small
		Bhvr	:PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		3 0 4

			Scale			1 1 1
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5

			InitialVelocity		0.3 0.5 0.3
			InitialVelocityJitter	0.2 0.3 0.4

			physRestitution 	0.7

			PhysDebris		0
		End
		CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
		CThresh 0.0001

		Lifespan	1220
		LifespanJitter	30
	End

	Event
		Ename	TireLeftFront
		Type	Start
		At	base

		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part


		Geom	carpart_wheel_small
		Bhvr	:PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-3 0 4

			Scale			1 1 1
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5

			InitialVelocity		-0.3 0.5 0.3
			InitialVelocityJitter	0.2 0.3 0.4

			physRestitution 	0.7

			PhysDebris		0
		End
		CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
		CThresh 0.0001

		Lifespan	1220
		LifespanJitter	30
	End


####################  Hood  ###############################

	Event
		Ename	Hood
		Type	Start
		At	base
		Part	:SmokeExplosion.part
		Part3	:FireBallSpark.part
		Part1	:FireBallTargetSteam.part

		Geom	carpart_hood
		Bhvr	:PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 1 3.5


			Scale			.6 .6 .6
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.35 0.35 0.35

			InitialVelocity		0.1 0.9 0.2
			InitialVelocityJitter	0.06 0.4 0.3

			PhysDebris		0

		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	1220
		LifespanJitter	30
	End

####################  Doors  ###############################

	Event
		Ename	DoorRight
		Type	Start
		At	base

		Part3	:FireBallSpark.part
		Part1	:FireBallTargetSteam.part

		Geom	carpart_Door_R
		Bhvr	:PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		3 1 0

			Scale			.8 .4 .8
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.35 0.35 0.35

			InitialVelocity		0.5 0.6 0.0
			InitialVelocityJitter	0.3 0.4 0.2

			PhysDebris		0
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	1220
		LifespanJitter	30
	End

	Event
		Ename	DoorLeft
		Type	Start
		At	base

		Part3	:FireBallSpark.part
		Part1	:FireBallTargetSteam.part

		Geom	carpart_Door_L
		Bhvr	:PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-3 1 0

			Scale			.8 .4 .8
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.35 0.35 0.35

			InitialVelocity		-0.5 0.6 0.0
			InitialVelocityJitter	0.3 0.4 0.2

			PhysDebris		0
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	1220
		LifespanJitter	30
	End


##################### License Plate  ######################

	Event
		Ename	Plate
		Type	Start
		At	base

		#Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part


		Geom	carpart_tag
		Bhvr	:PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 1 -5

			Scale			1 1 1
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5

			InitialVelocity		0.0 0.5 -0.5
			InitialVelocityJitter	0.2 0.4 0.3

			PhysDebris		0

		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	1220
		LifespanJitter	30
	End


####################  Bumpers  ###############################

	Event
		Ename	FrontBumper
		Type	Start
		At	base

		Part3	:FireBallSpark.part

		Geom	carpart_bumper_small
		Bhvr	:PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 1 4
			PyrRotate		0 3.14159 0

			Scale			.8 .8 .8
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.35 0.35 0.35

			InitialVelocity		0.0 0.4 0.6
			InitialVelocityJitter	0.2 0.3 0.3

			PhysDebris		0
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	1220
		LifespanJitter	30
	End

	Event
		Ename	RearBumper
		Type	Start
		At	base

		Part3	:FireBallSpark.part

		Geom	carpart_bumper_small
		Bhvr	:PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 0 -4.5

			Scale			.8 .8 .8
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.35 0.35 0.35

			InitialVelocity		0.0 0.4 -0.6
			InitialVelocityJitter	0.2 0.3 0.3

			PhysDebris		0
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	1220
		LifespanJitter	30
	End

####################  Fenders  ###############################

	Event
		Ename	FenderRightFront
		Type	Start
		At	base

		Part3	:FireBallSpark.part

		Geom	carpart_front_fender_R
		Bhvr	:PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		3 1 2

			Scale			.75 .75 .75
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.35 0.35 0.35

			InitialVelocity		0.3 0.6 0.2
			InitialVelocityJitter	0.3 0.4 0.2

			PhysDebris		0
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	1220
		LifespanJitter	30
	End

	Event
		Ename	FenderLeftFront
		Type	Start
		At	base

		Part3	:FireBallSpark.part

		Geom	carpart_front_fender_L
		Bhvr	:PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-3 1 2

			Scale			.75 .75 .75
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.35 0.35 0.35

			InitialVelocity		-0.3 0.6 0.2
			InitialVelocityJitter	0.3 0.4 0.2

			PhysDebris		0
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	1220
		LifespanJitter	30
	End

End

#######################################################################
#######################################################################

End