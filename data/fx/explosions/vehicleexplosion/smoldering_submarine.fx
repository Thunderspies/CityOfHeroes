#########################################################
##	car on fire
#########################################################

FxInfo  

Lifespan 1400

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	chest
		
#		Sound	flierexplo1 200 30 .93

	End

End

########  Main Explosion Particles   #########################


###################################################################

Condition


	Event
		EName 	base
		Type	local
		At	Chest
		BhvrOverride
			PositionOffset	0 3 -15
		End
		
		Part	:LargeCoreSmokeExplosion2.part
		
		Lifespan	800
		LifespanJitter	200
	End

	Event
		EName 	TailFin
		Type	local
		At	Chest
		BhvrOverride
			PositionOffset	0 8 -80
		End
		
		Part	:LargeCoreSmokeExplosion2.part
		
		Lifespan	800
		LifespanJitter	200
	End
	
	Event
		EName 	Center
		Type	local
		At	Chest
		BhvrOverride
			PositionOffset	0 5 5
		End
		
		Part	:LargeCoreSmokeExplosionLite2.part
		
		Lifespan	800
		LifespanJitter	200
	End


	Event
		EName 	Front
		Type	local
		At	Chest
		BhvrOverride
			PositionOffset	0 8 -40
		End
		
		Part	:LargeCoreSmokeExplosionLite.part
		
		Lifespan	800
		LifespanJitter	200
	End

	Event
		EName 	Tail
		Type	local
		At	Chest
		BhvrOverride
			PositionOffset	0 5 -100
		End
		
		Part	:LargeCoreSmokeExplosionLite2.part
		
		Lifespan	800
		LifespanJitter	200
	End
End

#########################################################################

########   Generic Explosion Physics Particles   ###################
  
Condition
	On 	time
	Time 	0
	Repeat 	12

	Event 
		EName 	Sparkz
		Type 	Start
		At 	Tail
		ChildFX V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
		Lifespan 60
	End
End

Condition
	On 	time
	Time 	0
	Repeat 	30

	Event 
		EName 	Sparkzz
		Type 	Start
		At 	Tail
		ChildFX V_COV\PhysicsEnabled\HotSparkExplode.fx
		Lifespan 60
	End
End


Condition
	On 	time
	Time 	0
	Repeat  21

	Event 
		EName 	napalm
		Type 	Local
		At 	Tail
		ChildFX V_COV\PhysicsEnabled\FireBurstExplode.fx
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
		At	Tail
		Geom	Metal_Curl
		Geom	Metal_Flat
		
		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part

		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.2 0.5
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	120
		LifespanJitter	30
	End

End

Condition
	On	Time
	Time 	3
	Repeat	15

	Event
		Ename	parts
		Type	Start
		At	Tail

		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part
		
		Part	V_COV\PhysicsEnabled\FireSplashBig.part
		
		Geom	carpart_sprocket
		Geom	carpart_pipe
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 0 2
			
			PyrRotateJitter		180 180 180

			Scale			1 1 1
			
			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5

			InitialVelocity		0.0 0.8 0.0
			InitialVelocityJitter	1.2 0.5 1.2

			Drag			0.01

			physSFriction 	 	0.6
			
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	120
		LifespanJitter	30
	End

End

########################################################################

Condition
	On 	time
	Time 	0
	Repeat 	12

	Event 
		EName 	Sparkz
		Type 	Start
		At 	base
		ChildFX V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
		Lifespan 60
	End
End

Condition
	On 	time
	Time 	0
	Repeat 	30

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
	Repeat  21

	Event 
		EName 	napalm
		Type 	Local
		At 	base
		ChildFX V_COV\PhysicsEnabled\FireBurstExplode.fx
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
		Part	:SmokeExplosion.part

		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.2 0.5
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	120
		LifespanJitter	30
	End

End

Condition
	On	Time
	Time 	3
	Repeat	15

	Event
		Ename	parts
		Type	Start
		At	base

		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part
		
		Part	V_COV\PhysicsEnabled\FireSplashBig.part
		
		Geom	carpart_sprocket
		Geom	carpart_pipe
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 0 2
			
			PyrRotateJitter		180 180 180

			Scale			1 1 1
			
			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5

			InitialVelocity		0.0 0.8 0.0
			InitialVelocityJitter	1.2 0.5 1.2

			Drag			0.01

			physSFriction 	 	0.6
			
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	120
		LifespanJitter	30
	End

End

########################################################################

Condition
	On 	time
	Time 	0
	Repeat 	12

	Event 
		EName 	Sparkz
		Type 	Start
		At 	TailFin
		ChildFX V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
		Lifespan 60
	End
End

Condition
	On 	time
	Time 	0
	Repeat 	30

	Event 
		EName 	Sparkzz
		Type 	Start
		At 	TailFin
		ChildFX V_COV\PhysicsEnabled\HotSparkExplode.fx
		Lifespan 60
	End
End


Condition
	On 	time
	Time 	0
	Repeat  21

	Event 
		EName 	napalm
		Type 	Local
		At 	TailFin
		ChildFX V_COV\PhysicsEnabled\FireBurstExplode.fx
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
		At	TailFin
		Geom	Metal_Curl
		Geom	Metal_Flat
		
		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part

		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.2 0.5
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	120
		LifespanJitter	30
	End

End

Condition
	On	Time
	Time 	3
	Repeat	15

	Event
		Ename	parts
		Type	Start
		At	TailFin

		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part
		
		Part	V_COV\PhysicsEnabled\FireSplashBig.part
		
		Geom	carpart_sprocket
		Geom	carpart_pipe
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 0 2
			
			PyrRotateJitter		180 180 180

			Scale			1 1 1
			
			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5

			InitialVelocity		0.0 0.8 0.0
			InitialVelocityJitter	1.2 0.5 1.2

			Drag			0.01

			physSFriction 	 	0.6
			
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	120
		LifespanJitter	30
	End

End


########################################################################

########################################################################

Condition
	On 	time
	Time 	0
	Repeat 	12

	Event 
		EName 	Sparkz
		Type 	Start
		At 	Center
		ChildFX V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
		Lifespan 60
	End
End

Condition
	On 	time
	Time 	0
	Repeat 	30

	Event 
		EName 	Sparkzz
		Type 	Start
		At 	Center
		ChildFX V_COV\PhysicsEnabled\HotSparkExplode.fx
		Lifespan 60
	End
End


Condition
	On 	time
	Time 	0
	Repeat  21

	Event 
		EName 	napalm
		Type 	Local
		At 	Center
		ChildFX V_COV\PhysicsEnabled\FireBurstExplode.fx
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
		At	Center
		Geom	Metal_Curl
		Geom	Metal_Flat
		
		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part

		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.2 0.5
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	120
		LifespanJitter	30
	End

End

Condition
	On	Time
	Time 	3
	Repeat	15

	Event
		Ename	parts
		Type	Start
		At	Center

		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part
		
		Part	V_COV\PhysicsEnabled\FireSplashBig.part
		
		Geom	carpart_sprocket
		Geom	carpart_pipe
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 0 2
			
			PyrRotateJitter		180 180 180

			Scale			1 1 1
			
			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5

			InitialVelocity		0.0 0.8 0.0
			InitialVelocityJitter	1.2 0.5 1.2

			Drag			0.01

			physSFriction 	 	0.6
			
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	120
		LifespanJitter	30
	End

End

#####################################################################################

Condition
	On 	time
	Time 	0
	Repeat 	12

	Event 
		EName 	Sparkz
		Type 	Start
		At 	Front
		ChildFX V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
		Lifespan 60
	End
End

Condition
	On 	time
	Time 	0
	Repeat 	30

	Event 
		EName 	Sparkzz
		Type 	Start
		At 	Front
		ChildFX V_COV\PhysicsEnabled\HotSparkExplode.fx
		Lifespan 60
	End
End


Condition
	On 	time
	Time 	0
	Repeat  21

	Event 
		EName 	napalm
		Type 	Local
		At 	Front
		ChildFX V_COV\PhysicsEnabled\FireBurstExplode.fx
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
		At	Front
		Geom	Metal_Curl
		Geom	Metal_Flat
		
		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part

		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.2 0.5
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	120
		LifespanJitter	30
	End

End

Condition
	On	Time
	Time 	3
	Repeat	15

	Event
		Ename	parts
		Type	Start
		At	Front

		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part
		
		Part	V_COV\PhysicsEnabled\FireSplashBig.part
		
		Geom	carpart_sprocket
		Geom	carpart_pipe
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 0 2
			
			PyrRotateJitter		180 180 180

			Scale			1 1 1
			
			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5

			InitialVelocity		0.0 0.8 0.0
			InitialVelocityJitter	1.2 0.5 1.2

			Drag			0.01

			physSFriction 	 	0.6
			
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	120
		LifespanJitter	30
	End

End

End			