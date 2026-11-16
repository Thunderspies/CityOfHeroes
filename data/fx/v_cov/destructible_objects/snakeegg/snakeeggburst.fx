
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
			PositionOffset	0 4 0
		End
		LifeSpan 	10
		Sound eggexplo 88 88 .9
		Part1	:EggShellBurst.part
		Part1	:ExplosiveFlash.part
		Part1	:EggSlimeSplash.part
		Part1	:EggSlimeDrip.part
		Part1	:EggShellMist.part
	End

End

#############  Shell Chunks

Condition
	On 	Time
	Time 	0
	Repeat 		40
	RepeatJitter	10
	
	Event
		Type	start
		At	Center
		Bhvr	:EggShellDebris.bhvr
		geom	_v_dest_egg_piece01
		geom	_v_dest_egg_piece02
		geom	_v_dest_egg_piece03
		geom	_v_dest_egg_piece05
		part	:EggSlimeTrail.part
		
		LifeSpan 	240
		LifeSpanJitter 	60
	End

End

End