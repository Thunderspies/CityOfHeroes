
FxInfo

Lifespan	300

Condition
	On 	Time
	Time 	0

	Event
		Ename	Center
		Type	Local 
		At	Root
		BhvrOverride
			PositionOffset	0 5 0
		End
		LifeSpan 	10
		Sound eggexplo 88 88 .9
		Part1	:GrowthBombShellBurst.part
		Part1	:ExplosiveFlash.part
		Part1	:GrowthBombSlimeSplash.part
		Part1	:GrowthBombSlimeDrip.part
		Part1	:GrowthBombShellMist.part
	End

End

#############  Metal Chunks

Condition
	On 	Time
	Time 	0
	Repeat 		40
	RepeatJitter	10
	
	Event
		Type	start
		At	Center

		BhvrOverride	
			StartJitter		1.0 1.0 1.0
			PositionOffset		0 5 0
			Startcolor		200 230 200
			Scale			.05 .05 .05
			ScaleRate		.05 .05 .05
			EndScale		.03 .03 .03
		End

		Bhvr	V_COV\Destructible_Objects\SnakeEgg\EggShellDebris.bhvr
		
		Geom	Metal_Curl
		Geom	Metal_Flat

		part	:GrowthBombSlimeTrail.part
		
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		LifeSpan 	240
		LifeSpanJitter 	60
	End

End

End