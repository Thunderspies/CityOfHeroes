#########################################################
##	car on fire
#########################################################

FxInfo

Lifespan 300

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime1
		Type	Local
		At	chest

#		Sound car_explo1 80 80 .9

	End

End

########   Explosion Force   #########################

Condition
	On	Time
	Time	0

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest

		BhvrOverride

			PhysForceType		Out
			PhysForceRadius		30
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
			PositionOffset	0 5 0
		End

	End



	Event
		EName 	Sound
		Type	Posit
		At	base
		Sound molotovhit1 70 70 .86
		Sound mailboxexplo3 70 70 1.0

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
#		BhvrOverride
#			StartJitter		2 1 2
#		End
		ChildFX V_COV\PhysicsEnabled\HotSparkExplode.fx
		Lifespan 60
	End

#	Event
#		EName 	Sparkzz2
#		HardwareOnly
#		Type 	Start
#		At 	base
#		ChildFX V_COV\PhysicsEnabled\HotSparkExplode.fx
#		Lifespan 60
#	End
#
#	Event
#		EName 	Sparkzz3
#		HardwareOnly
#		Type 	Start
#		At 	base
#		ChildFX V_COV\PhysicsEnabled\HotSparkExplode.fx
#		Lifespan 60
#	End
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


#	Event
#		EName 	napalm2
#		HardwareOnly
#		Type 	Local
#		At 	base
#		ChildFX V_COV\PhysicsEnabled\FireBurstScatter.fx
#		Lifespan 70
#		LifespanJitter 	20
#	End
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
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.2 0.5
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	120
		LifespanJitter	30
	End

End

Condition
	On	Time
	Time 	3
	Repeat	9

#	Event
#		Ename	parts
#		Type	Start
#		At	base
#
#		Part3	:FireBallSpark.part
#
#		Part	V_COV\PhysicsEnabled\FireSplashBig.part
#
#		Geom	carpart_sprocket
#		Geom	carpart_pipe
#
#		Bhvr	:SkiffPhysicsDebrisFade.bhvr
#		BhvrOverride
#			PositionOffset		0 1 2
#
#			PyrRotateJitter		180 180 180
#
#			Scale			0.6 0.6 0.6
#
#			Spin			0.0 0.0 0.0
#			SpinJitter		0.5 0.5 0.5
#
#			InitialVelocity		0.0 0.5 0.0
#			InitialVelocityJitter	1.2 0.8 1.2
#
#			Drag			0.01
#
#			physSFriction 	 	0.6
#
#		End
#		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
#		CThresh 0.0001
#
#		Lifespan	120
#		LifespanJitter	30
#	End

	Event
		Ename	parts2
		HardwareOnly
		Type	Start
		At	base

		Part3	:FireBallSpark.part

		Part	V_COV\PhysicsEnabled\FireSplashBig.part

		Geom	carpart_sprocket
		Geom	carpart_pipe

		Bhvr	:SkiffPhysicsDebrisFade.bhvr
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

			PhysDebris		1

		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	10
	End

End

Condition
	On	Time
	Time 	3
	Repeat	8  #12

#	Event
#		Ename	parts3
#		HardwareOnly
#		Type	Start
#		At	base
#
#		Part3	:FireBallSpark.part
#
#		Part	V_COV\PhysicsEnabled\FireSplashBig.part
#
#		Geom	Metal_Flat01
#		Geom	Metal_Flat02
#		Geom	Metal_Flat03
#		Geom	Widget_01
#		Geom	Widget_02
#		Geom	Widget_03
#		#Geom	Widget_04
#		Geom	Widget_05
#		Geom	Widget_06
#		Geom	Widget_07
#		Geom	Widget_08
#		Geom	Widget_09
#		Geom	Widget_10
#		Geom	Widget_12
#		Geom	Widget_13
#		Geom	Widget_14
#
#		Bhvr	:SkiffPhysicsDebrisFade.bhvr
#		BhvrOverride
#			PositionOffset		0 1 2
#
#			PyrRotateJitter		180 180 180
#
#			Scale			.5 .5 .5
#
#			Spin			0.0 0.0 0.0
#			SpinJitter		0.5 0.5 0.5
#
#			InitialVelocity		0.0 0.5 0.0
#			InitialVelocityJitter	1.2 0.8 1.2
#
#			Drag			0.01
#
#			physSFriction 	 	0.6
#
#			PhysDebris		1
#
#		End
#		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
#		CThresh 0.0001
#
#		Lifespan	60
#	End

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

		Bhvr	:SkiffPhysicsDebrisFade.bhvr
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

			PhysDebris		1

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

		Geom	exp_LBV_fuselage
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			Scale			1 1 1
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.06 0.05 0.07

			InitialVelocity		0.0 0.65 -0.0
			InitialVelocityJitter	0.15 0.2 0.15

			physRestitution 	0.25

			#PhysDebris		1
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks_LongbowSkiff_Explo.fx
		CThresh 0.0001

		Lifespan	210
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

	Event
		EName 	EngineFire
		Type	Local
		At	Husk
		BhvrOverride
			PositionOffset	0 1 -1.5
		End
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part

	End

##################### Engines  ######################

	Event
		Ename	NacelleRightBig
		Type	Start
		At	base

		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part


		Geom	exp_LBV_Engine
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		5 2 -1

			Scale			1 1 1

			PyrRotate		75 0 0
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5

			InitialVelocity		.3 .5 -.1 #0.3 0.5 -0.3
			InitialVelocityJitter	0.2 0.3 0.4

			physRestitution 	0.7

			PhysDebris		1 #3
		End
		#CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
		CThresh 0.0001

		Lifespan	30
	End

	Event
		Ename	NacelleLeftBig
		Type	Start
		At	base

		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part


		Geom	exp_LBV_Engine
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-5 2 -1

			Scale			1 1 1

			PyrRotate		75 0 0
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5

			InitialVelocity		-.3 .5 -.1 #-0.3 0.5 -0.3
			InitialVelocityJitter	0.2 0.3 0.4

			PhysDebris		1 #3
		End
		#CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
		CThresh 0.0001

		Lifespan	30
	End

	Event
		Ename	NacelleRightSmall
		Type	Start
		At	base

		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part


		Geom	exp_LBV_Engine
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		3 2 -1

			Scale			.6 .6 .6
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5

			InitialVelocity		0.1 0.7 -0.5
			InitialVelocityJitter	0.2 0.3 0.4

			PhysDebris		1 #3
		End
		#CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
		CThresh 0.0001

		Lifespan	30
	End

	Event
		Ename	NacelleLeftSmall
		Type	Start
		At	base

		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part


		Geom	exp_LBV_Engine
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-3 2 -1

			Scale			.6 .6 .6
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5

			InitialVelocity		-0.1 0.7 -0.5
			InitialVelocityJitter	0.2 0.3 0.4

			physRestitution 	0.7

			PhysDebris		1 #3
		End
		#CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
		CThresh 0.0001

		Lifespan	30
	End


####################  Canopy  ###############################

	Event
		Ename	Canopy
		Type	Start
		At	base
		Part	:SmokeExplosion.part
		Part3	:FireBallSpark.part
		Part1	:FireBallTargetSteam.part

		Geom	exp_LBV_canopy
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 3 4

			Alpha			150

			Scale			1 1 1
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.15 0.25 0.35

			InitialVelocity		0.0 0.8 0.4
			InitialVelocityJitter	0.1 0.3 0.2

		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks_LongbowSkiff_Explo.fx
		CThresh 0.0001

		Lifespan	120
		LifespanJitter	30
	End

####################  Wings  ###############################

	Event
		Ename	WingRight
		Type	Start
		At	base

		Part3	:FireBallSpark.part
		Part1	:FireBallTargetSteam.part

		Geom	exp_LBV_wing_R
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		8 1 -1

			Scale			1 1 1
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.35 0.35 0.35

			InitialVelocity		0.7 0.6 0.2
			InitialVelocityJitter	0.3 0.4 0.3

			PhysDebris		1
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	30
	End

	Event
		Ename	WingLeft
		Type	Start
		At	base

		Part3	:FireBallSpark.part
		Part1	:FireBallTargetSteam.part

		Geom	exp_LBV_wing_L
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-8 1 -1

			Scale			1 1 1
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.35 0.35 0.35

			InitialVelocity		-0.7 0.6 0.2
			InitialVelocityJitter	0.3 0.4 0.3

			PhysDebris		1
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	30
	End


##################### TailRotors  ######################

	Event
		Ename	TailRotorRight
		Type	Start
		At	base

		Part1	:FireBallTargetSteam.part
		#Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part


		Geom	exp_LBV_propeller
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		1 1 -6

			Scale			1 1 1
			PyrRotateJitter		11 11 11

			Spin			0.0 -9.9 0.0
			#SpinJitter		0.15 0.25 0.15

			InitialVelocity		0.2 0.9 -0.3
			InitialVelocityJitter	0.2 0.3 0.3

			Gravity			.03
			PhysGravity		.7

		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks_LongbowSkiff_Explo.fx
		CThresh 0.0001

		Lifespan	180
		LifespanJitter	30
	End

	Event
		Ename	TailRotorLeft
		Type	Start
		At	base

		Part1	:FireBallTargetSteam.part
		#Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part


		Geom	exp_LBV_propeller
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-1 1 -6

			Scale			1 1 1

			PyrRotate		0 0 180
			PyrRotateJitter		11 11 11

			Spin			0.0 9.9 0.0
			#SpinJitter		0.15 0.25 0.15

			InitialVelocity		-0.2 0.9 -0.3
			InitialVelocityJitter	0.2 0.3 0.3

			Gravity			.03
			PhysGravity		.7

		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks_LongbowSkiff_Explo.fx
		CThresh 0.0001

		Lifespan	180
		LifespanJitter	30
	End


####################  GunPods  ###############################

	Event
		Ename	GunPodRight
		Type	Start
		At	base

		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part

		Geom	exp_LBV_gunpod_R
		Bhvr	:SkiffPhysicsDebris.bhvr
		BhvrOverride
			PositionOffset		1.5 -2 2

			Scale			1 1 1
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.35 0.35 0.35

			InitialVelocity		0.3 -0.4 0.4
			InitialVelocityJitter	0.3 0.4 0.2

		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks_LongbowSkiff_Explo.fx
		CThresh 0.0001

		Lifespan	150
		LifespanJitter	30
	End

	Event
		Ename	GunPodLeft
		Type	Start
		At	base

		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part

		Geom	exp_LBV_gunpod_L
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-1.5 -2 2

			Scale			1 1 1
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.35 0.35 0.35

			InitialVelocity		-0.3 -0.4 0.4
			InitialVelocityJitter	0.3 0.4 0.2

		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks_LongbowSkiff_Explo.fx
		CThresh 0.0001

		Lifespan	120
		LifespanJitter	30
	End

	Event
		Ename	Prime
		Type	Local
		At	base

		Lifespan	75
		LifespanJitter	45
	End


End

####################################################
##################  GunPod Ammo  #############
####################################################

Condition
	On 	Cycle
	Time 	3

	Chance	.25

	Event
		EName 	Blast1
		Type	local

		At	GunPodRight
		BhvrOverride
			PositionOffset	0 0 2
		End
		Part1	ENEMYFX\SkyRaiders\MachineGunFlash.part
		Part2	ENEMYFX\SkyRaiders\MachineGunGlow.part
		Lifespan	3
	End

End


Condition
	On 	Cycle
	Time 	3

	Chance	.25

	Event
		EName 	Blast2
		Type	local

		At	GunPodLeft
		BhvrOverride
			PositionOffset	0 0 2
		End
		Part1	ENEMYFX\SkyRaiders\MachineGunFlash.part
		Part2	ENEMYFX\SkyRaiders\MachineGunGlow.part

		sound	exploblast 70 70 .5
		sound	explomed2 70 70 .6
		sound	fireexplo4 200 200 .5
		sound	fireexplo5 200 200 .5

		Lifespan	3
	End

End

Condition
	On 	PrimeDied

	Chance	.67

	Event
		EName 	Explode1
		Type	start

		At	GunPodRight
		ChildFX	Explosions\medium\Expolsion_child.fx

		Lifespan	120
	End

	Event
		EName 	GunPodRight
		Type	Destroy
	End
End


#######################################################################
#######################################################################

End