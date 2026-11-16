#############################################################
## FX System Name
#############################################################

FxInfo

## INITIAL ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin
		Part1	:LargeBeam_Bolt01.part
		Part1	:LargeBeam_Bolt02.part
		Part1	:LargeBeam_Bolt_Glow1.part
		pOther	Chest
	End		

	Event
		Type	Local
		At	Origin
		Part1	:LargeBeam_Bolt04.part
		Part1	:LargeBeam_Bolt06.part
		Part1	:LargeBeam_Bolt_Glow2.part
		pOther	Chest
	End		

End

## CYCLE ###########################################################

Condition
	On 	Cycle
	Time 	3

	Event
		Type	Local
		At	Origin
		Part1	:LargeBeam_Bolt02.part
		Part1	:LargeBeam_Bolt06.part
		Part1	:LargeBeam_Bolt_Glow1.part
		Part1	:LargeBeam_Bolt_Glow2.part
		pOther	Chest
		Lifespan	60
	End		
End

## RANDOM ###########################################################

Condition
	On 	Cycle
	Time 	3
	Chance	0.25

	Event
		Type	Local
		At	Origin
		Part1	:LargeBeam_Bolt01.part
		Part1	:LargeBeam_Bolt02.part
		Part1	:LargeBeam_Bolt_Glow2.part
		pOther	Chest
		Lifespan	60
	End		
End

Condition
	On 	Cycle
	Time 	3
	Chance	0.25

	Event
		Type	Local
		At	Origin
		Part1	:LargeBeam_Bolt03.part
		Part1	:LargeBeam_Bolt05.part
		Part1	:LargeBeam_Bolt_Glow1.part
		pOther	Chest
		Lifespan	60
	End		
End


Condition
	On 	Cycle
	Time 	3
	Chance	0.25

	Event
		Type	Local
		At	Origin
		Part1	:LargeBeam_Bolt04.part
		Part1	:LargeBeam_Bolt06.part
		Part1	:LargeBeam_Bolt_Glow2.part
		pOther	Chest
		Lifespan	60
	End		
End

#############################################################

End