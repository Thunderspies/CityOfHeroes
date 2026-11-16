
FxInfo

Lifespan	300


Condition

	On	Time
	Time	0

	Event
		Type	Local
		At	origin
		Sound cocoonexplo2 130 130 .89
	End
End


Condition
	On 	Time
	Time 	0

	Event
		Ename	Center
		Type	Local 
		At	Root
		BhvrOverride
			PositionOffset	0 4 0
		End
		LifeSpan 	10
		
		Part1	:CacoonDripsBurst.part
		Part1	:ExplosiveFlash.part
		Part1	:CacoonSlimeSplash.part
		Part1	:CacoonBurst.part
		Part1	:CacoonShellMist.part
	End

End

#############  Shell Chunks

Condition
	On 	Time
	Time 	0
	Repeat 		40
	RepeatJitter	10
	
	Event
		Ename	cacoonBit
		Type	start
		At	Center
		Bhvr	:CacoonlDebris.bhvr
		geom	cacoonBit_01
		geom	cacoonBit_02
		part	:CacoonSlimeTrail.part
		
		LifeSpan 	140
		LifeSpanJitter 	60
	End

End

End