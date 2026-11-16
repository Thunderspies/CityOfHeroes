#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

Condition

	On	Time
	Time	22

	Event
		Ename	Prime
		Type	start 
		At	chest
		bhvr	behaviors/projectile3.bhvr
		geom	centerdummy
		
		magnet	target
		lookat	target
		Sound graniteexplo 88 88 .77

	End

	Event

		Type	Local 
		At	mystic
		Part	:Gravel_Rocks_01.part
		Part	:Gravel_Rocks_Dark.part
	End

	Event

		Type	Local 
		At	WepR
		Part	:Gravel_Dust.part
	End

	Event

		Type	Local 
		At	WepL
		Part	:Gravel_Dust.part
	End

End

Condition
	On	Time
	Time	32

	Event

		Type	Local 
		At	mystic
		Part	:Gravel_Rocks_01.part
		Part	:Gravel_Rocks_Dark.part
		Sound saltexplo 88 88 .5

		
	End

	Event

		Type	Local 
		At	WepR
		Part	:Gravel_Dust.part
	End

	Event

		Type	Local 
		At	WepL
		Part	:Gravel_Dust.part
	End

End


Condition
	On	Time
	Time	33

	Event

		Type	Local 
		At	mystic
		Part	:Gravel_Rocks_01.part
		Part	:Gravel_Rocks_Dark.part
		
	End

	Event

		Type	Local 
		At	WepR
		Part	:Gravel_Dust.part
	End

	Event

		Type	Local 
		At	WepL
		Part	:Gravel_Dust.part
	End

End

Condition
	On	Time
	Time	35

	Event

		Type	Local 
		At	mystic
		Part	:Gravel_Rocks_01.part
		Part	:Gravel_Rocks_Dark.part
		
	End

	Event

		Type	Local 
		At	WepR
		Part	:Gravel_Dust.part
	End

	Event

		Type	Local 
		At	WepL
		Part	:Gravel_Dust.part
	End

End

Condition
	On	Time
	Time	40

	Event

		Type	Local 
		At	mystic
		Part	:Gravel_Rocks_01.part
		Part	:Gravel_Rocks_Dark.part
		
	End

End

End	


			