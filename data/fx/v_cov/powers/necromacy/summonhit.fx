#############################################################
## SummonHit.fx
#############################################################

FxInfo

lifeSpan 180

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		ENAME	Base
		Type	start
		At	root
		Bhvr 	:DarkScale3.bhvr
		Geom	FX_DarkWave
		Sound miasmaburst 100 100 0.6
		lifeSpan 100
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	darkskirt
		Type	Start
		At	ROOT
		Part	:2CreateDarknessPuddle2.part
		Part	powers/DarknessControl/CreateDarknessPuddle2Flat_Subtractive.part
		Part	powers/DarknessControl/CreateDarknessPuddle2Flat.part
		Part	:2CreateDarknessPuddle.part
		lifeSpan 80
	End

	Event

		ENAME	hookupW/Spin
		Type	posit
		At	T_root
		BHVR	POWERS\DarknessControl\SplatRotate.bhvr
		lifeSpan 20
	End
End

############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	BaseTrail
		Type	start
		At	hookupW/Spin
		ChildFX	:Child_SummonHit_Splats.fx
		Lifespan 20
	End
End

#############################################################

End