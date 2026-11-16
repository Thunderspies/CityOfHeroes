#########################################################
## Tar Patch
#########################################################

FxInfo

#Lifespan	60

#########################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		ENAME	hookupW/Spin
#		Type	start
#		At	root
#		BHVR	powers\DarknessControl\SplatRotate.bhvr
#	End
#End
#
#Condition
#	On	cycle
#	Time	15
#
#	Event
#
#		ENAME	BaseTrail
#		Type	Posit
#		At	hookupW/Spin
#		BHVR	powers\DarknessControl\Darktendril.bhvr
#		Splat	DarknessSplat.tga
#		LifeSpan	30
#	End
#
#	Event
#		ENAME	BaseTrail
#		Type	Posit
#		At	hookupW/Spin
#		BHVR	powers\DarknessControl\DarktendrilIn.bhvr
#		Splat	DarknessSplat2.tga
#		LifeSpan	30
#	End
#
#	Event
#		ENAME	BaseTrail
#		Type	Posit
#		At	hookupW/Spin
#		BHVR	powers\DarknessControl\DarkSplat2.bhvr
#		Splat	Ember02.tga
#		LifeSpan	30
#	End
#End

#Condition
#	On	cycle
#	Time	19
#
#	Event
#		ENAME	BaseTrail
#		Type	start
#		At	root
#		Bhvr	powers\DarknessControl\splat.bhvr
#		splat	Generic_Ring.tga
#		LifeSpan	30
#	End
#End

#########################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	BaseTrail
		Type	start
		At	root
		Part1	powers\DarknessControl\DarkTarRing.Part
		Part1	powers\DarknessControl\DarkPuddle.part
		Part2	powers\DarknessControl\DarkPuddle2.part
		Part4	powers\DarknessControl\DarknessTendrill.Part
		Sound Tar_loop 80 80 .53
	End
End

Condition
	On	Time
	Time	0

	Event
		EName 	burst
		Type	start
		At	Root
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		Part1	powers\DarknessControl\PitBust1.part
		Part2	powers\DarknessControl\PitBust2.part
		Sound grexplo2 80 80 1.0
	End

	Event
		ENAME	Base
		Type	start
		At	root
		Bhvr 	Behaviors/DarkScale2.bhvr
		Geom	FX_DarkWave
	End
End

Condition
	On	Time
	Time	30

	Event
		ENAME	BaseTrail
		Type	start
		At	root
		Part3	powers\DarknessControl\TarBubblePopping.part
		Part5	powers\DarknessControl\TarBubblePoppingFlat.part
		Sound miasmaburst4 80 80 .62
	End
End

#########################################################

End