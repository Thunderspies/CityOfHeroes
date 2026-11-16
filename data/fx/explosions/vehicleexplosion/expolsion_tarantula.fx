#########################################################
##	car on fire
#########################################################

FxInfo  

Lifespan 300


Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	chest
		
		Sound tarantula_explo 160 160 .93

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
		Inherit	Position Rotation Scale
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
			PositionOffset	0 -1 2
		End

		LifeSpan	20
	
	End

	Event
		EName 	Flames1
		Type	Local
		At	Offset1
		ChildFX	med_fire.fx

		LifeSpan	20
	
	End

	Event
		EName 	Offset2
		Type	local
		At	base
		BhvrOverride
			PositionOffset	0 1 -2
		End

		LifeSpan	20
	
	End

	Event
		EName 	Flames2
		Type	local
		At	Offset2
		ChildFX	med_fire.fx

		LifeSpan	20
	
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
		
		Lifespan 6
	End

	Event
		EName 	Explosion2
		Type	local
		At	base
		Part1	:FireExplosion.part
		Part2	:Inferno2.part
		
		Lifespan 10
	End

	Event
		EName 	Flash
		Type	local
		At	base
	
		Part	:FireExplosionFlash.part
		Part	:ExplosionStar.part
		Part	:ExplosionlightStreams.part

		Lifespan 3
		
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
	Repeat	7

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
#		Bhvr	:PhysicsDebrisFade.bhvr
#		BhvrOverride
#			PositionOffset		0 3 0
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
#		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
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

		Bhvr	:PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 3 0
			
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
#		#Geom	Widget_01
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
#		Bhvr	:PhysicsDebrisFade.bhvr
#		BhvrOverride
#			PositionOffset		0 3 0
#			
#			PyrRotateJitter		180 180 180
#
#			Scale			.4 .4 .4
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
		#Geom	Widget_01
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
			PositionOffset		0 3 0
			
			PyrRotateJitter		180 180 180

			Scale			.4 .4 .4
			
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
		Ename	Body
		Type	Start
		At	base
		
		Part	:SmokeExplosion.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		Part1	:FireBallTargetSteam.part
				
		Geom	exp_TARAN_body
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			Scale			1 1 1
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.06 0.05 0.07

			InitialVelocity		0.0 0.65 -0.0
			InitialVelocityJitter	0.15 0.2 0.15

			physRestitution 	0.25

			PhysDebris		1
		End
		
		CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
		CThresh 0.0001
		
		Lifespan	30
	End


#####################  Tail  #########################

	Event
		Ename	Husk
		Type	Start
		At	base
		Part	:SmokeExplosion.part
		
		ChildFX	med_fire.fx
		
		Geom	exp_TARAN_abdomen
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 3 -3

			Scale			1 1 1
			PyrRotateJitter		0 0 0

			Spin			-0.09 0.0 0.0
			SpinJitter		0.06 0.05 0.07

			InitialVelocity		0.0 0.65 -0.3
			InitialVelocityJitter	0.15 0.3 0.25

			physRestitution 	0.25

			PhysDebris		1 #3
		End
		
		#CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
		CThresh 0.0001
		
		Lifespan	75
		lifespanJitter	30
	End

	Event
		EName 	EngineFire
		Type	Local
		At	Husk
		BhvrOverride
			PositionOffset	0 0 -2
		End
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part
			
	End

####################  Head  ###############################

	Event
		Ename	Head
		Type	Start
		At	base
		Part	:SmokeExplosion.part
		Part3	:FireBallSpark.part
		Part1	:FireBallTargetSteam.part

		ChildFX	small_fire.fx
		
		Geom	exp_TARAN_head
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 1 2
		
			Scale			1 1 1
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.15 0.25 0.35

			InitialVelocity		0.0 0.8 0.4
			InitialVelocityJitter	0.1 0.3 0.2

			PhysDebris		1 #3
		End
		
		#CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
		CThresh 0.0001
		
		Lifespan	30
	End


##################### Pedipalps  ######################

	Event
		Ename	PedipalpRight
		Type	Start
		At	base
		
		Part2	V_COV\PhysicsEnabled\FireSplashBig.part
		Part3	:FireBallSpark.part
		
		
		Geom	exp_TARAN_pedipalp
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		.5 -1 3
		
			Scale			1 1 1
			PyrRotateJitter		11 11 11

			SpinJitter		0.15 0.25 0.15

			InitialVelocity		0.2 0.2 0.6
			InitialVelocityJitter	0.2 0.3 0.3

			Gravity			.03
			PhysGravity		.7

			PhysDebris		1 #3

		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	18
		LifespanJitter	3
	End

	Event
		Ename	PedipalpLeft
		Type	Start
		At	base
		
		Part2	V_COV\PhysicsEnabled\FireSplashBig.part
		Part3	:FireBallSpark.part
		
		
		Geom	exp_TARAN_pedipalp
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-.5 -1 3
		
			Scale			1 1 1
			
			PyrRotateJitter		11 11 11

			SpinJitter		0.15 0.25 0.15

			InitialVelocity		-0.2 0.2 0.6
			InitialVelocityJitter	0.2 0.3 0.3

			Gravity			.03
			PhysGravity		.7
			
			PhysDebris		1 #3

		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	18
		LifespanJitter	3
	End


####################  GunPods  ###############################

	Event
		Ename	GunPodRight
		Type	Start
		At	base

		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		
		Geom	exp_TARAN_gunpod
		Bhvr	:SkiffPhysicsDebris.bhvr
		BhvrOverride
			PositionOffset		1 -2 2
		
			Scale			1 1 1
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.35 0.35 0.35

			InitialVelocity		0.4 -0.2 0.4
			InitialVelocityJitter	0.3 0.3 0.2

		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
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
		
		Geom	exp_TARAN_gunpod
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-1 -2 2
	
			Scale			1 1 1
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.35 0.35 0.35

			InitialVelocity		-0.4 -0.2 0.4
			InitialVelocityJitter	0.3 0.3 0.2

		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	120
		LifespanJitter	30
	End

####################  Legs  ###############################

	Event
		Ename	LegRight1
		Type	Start
		At	base

		Part3	:FireBallSpark.part
		Part1	:FireBallTargetSteam.part
		
		Geom	exp_TARAN_leg
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		2 0 2
	
			Scale			1 1 1

			PyrRotate		0 30 0
			PyrRotateJitter		10 20 10

			Spin			0.0 0.0 0.0
			SpinJitter		0.15 0.15 0.15

			InitialVelocity		0.4 0.6 0.5
			InitialVelocityJitter	0.3 0.4 0.3

			PhysDebris		1
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	30
	End

	Event
		Ename	LegRight2
		Type	Start
		At	base

		Part3	:FireBallSpark.part
		Part1	:FireBallTargetSteam.part
		
		Geom	exp_TARAN_leg
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		3 0 1
	
			Scale			1 1 1
			
			PyrRotate		0 70 0
			PyrRotateJitter		10 20 10

			Spin			0.0 0.0 0.0
			SpinJitter		0.15 0.15 0.15

			InitialVelocity		0.7 0.6 0.2
			InitialVelocityJitter	0.3 0.4 0.3

			PhysDebris		1
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	30
	End

	Event
		Ename	LegRight3
		Type	Start
		At	base

		Part3	:FireBallSpark.part
		Part1	:FireBallTargetSteam.part
		
		Geom	exp_TARAN_leg
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		3 0 -1
	
			Scale			1 1 1

			PyrRotate		0 120 0
			PyrRotateJitter		10 20 10

			Spin			0.0 0.0 0.0
			SpinJitter		0.15 0.15 0.15

			InitialVelocity		0.7 0.6 -0.2
			InitialVelocityJitter	0.3 0.4 0.3

			PhysDebris		1
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	30
	End

	Event
		Ename	LegRight4
		Type	Start
		At	base

		Part3	:FireBallSpark.part
		Part1	:FireBallTargetSteam.part
		
		Geom	exp_TARAN_leg
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		2 0 -3
	
			Scale			1 1 1

			PyrRotate		0 160 0
			PyrRotateJitter		10 20 10

			Spin			0.0 0.0 0.0
			SpinJitter		0.15 0.15 0.15

			InitialVelocity		0.4 0.6 -0.5
			InitialVelocityJitter	0.3 0.4 0.3

			PhysDebris		1
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	30
	End

Event
		Ename	LegLeft1
		Type	Start
		At	base

		Part3	:FireBallSpark.part
		Part1	:FireBallTargetSteam.part
		
		Geom	exp_TARAN_leg
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-2 0 2
	
			Scale			1 1 1

			PyrRotate		0 -30 0
			PyrRotateJitter		10 20 10

			Spin			0.0 0.0 0.0
			SpinJitter		0.15 0.15 0.15

			InitialVelocity		-0.4 0.6 0.5
			InitialVelocityJitter	0.3 0.4 0.3

			PhysDebris		1
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	30
	End

	Event
		Ename	LegLeft2
		Type	Start
		At	base

		Part3	:FireBallSpark.part
		Part1	:FireBallTargetSteam.part
		
		Geom	exp_TARAN_leg
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-3 0 1
	
			Scale			1 1 1
			
			PyrRotate		0 -70 0
			PyrRotateJitter		10 20 10

			Spin			0.0 0.0 0.0
			SpinJitter		0.15 0.15 0.15

			InitialVelocity		-0.7 0.6 0.2
			InitialVelocityJitter	0.3 0.4 0.3

			PhysDebris		1
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	30
	End

	Event
		Ename	LegLeft3
		Type	Start
		At	base

		Part3	:FireBallSpark.part
		Part1	:FireBallTargetSteam.part
		
		Geom	exp_TARAN_leg
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-3 0 -1
	
			Scale			1 1 1

			PyrRotate		0 -120 0
			PyrRotateJitter		10 20 10

			Spin			0.0 0.0 0.0
			SpinJitter		0.15 0.15 0.15

			InitialVelocity		-0.7 0.6 -0.2
			InitialVelocityJitter	0.3 0.4 0.3

			PhysDebris		1
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	30
	End

	Event
		Ename	LegLeft4
		Type	Start
		At	base

		Part3	:FireBallSpark.part
		Part1	:FireBallTargetSteam.part
		
		Geom	exp_TARAN_leg
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-2 0 -3
	
			Scale			1 1 1

			PyrRotate		0 -160 0
			PyrRotateJitter		10 20 10

			Spin			0.0 0.0 0.0
			SpinJitter		0.15 0.15 0.15

			InitialVelocity		-0.4 0.6 -0.5
			InitialVelocityJitter	0.3 0.4 0.3

			PhysDebris		1
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	30
	End



End

##################### Arms  ######################
Condition
	Time 	0
	Repeat	5
	RepeatJitter	2

	Event
		Ename	Arm
		Type	Start
		At	base
		
		Part1	:FireBallTargetSteam.part
		Part2	V_COV\PhysicsEnabled\FireSplashBig.part #:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		
		
		Geom	exp_TARAN_arm
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			StartJitter		3 1 4 
			
			Scale			1 1 1
			
			PyrRotateJitter		45 45 45

			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5

			InitialVelocity		0 .5 0
			InitialVelocityJitter	0.5 0.3 0.4

			physRestitution 	0.6

			PhysDebris		1 #3
			
		End

		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	21
		LifespanJitter	3
	End

End
#######################################################################
#######################################################################

End			