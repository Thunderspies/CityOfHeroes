#########################################################
##	template

FxInfo

LifeSpan	95

Condition
	On 	Time
	Time 	14

	Event
		Type	local
		At	Origin
		Sound Whooshbig3 100.0 100.0 .8
	End

End

Condition
	On 	Time
	Time 	51

	Event
		EName	Streak
		Type	Start
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Medieval1.bhvr
		Sound Whoosh5 100.0 100.0 .6
		Lifespan 1
	End

End

Condition
	On 	Time
	Time 	52

	Event
		EName	Streak
		Type	local
		At	WepR
		
		bhvr	behaviors/Katana.bhvr
		Part	:KatanaTrail2.part
		
	End

	Event
		EName	Streak
		Type	local
		At	WepR
		
		Part	:KatanaTrail2.part
				
	End

End

Condition
	On 	Time
	Time 	49

	
	Event
		
		Type	local
		At	WepR
				
		Part	:KatanaBright2.part
		Part	:KatanaBright2.part

		bhvr	behaviors/Katanabanner.bhvr
		Lifespan	6
	End

	Event
		
		Type	local
		At	WepR
		
		Part	:KatanaBright2.part	
		Part	:KatanaBright2.part
		bhvr	behaviors/KatanabannerBottem.bhvr
		Lifespan	6
	End


	Event
		
		Type	local
		At	WepR

		Part	:KatanaBright2.part		
		Part	:KatanaBright2.part
		bhvr	behaviors/Katanabannertop.bhvr
		Lifespan	6
	End

	Event
		EName	Streak
		Type	local
		At	WepR
		
		Part	:KatanaElectricity.part
		Lifespan	85
	End

End

End	

	