#############################################################
## LampLight.fx
#############################################################

FxInfo
Lifespan 30

#############################################################

Condition
	On 	Cycle
	Time 	5
	Chance	0.5


	Event
		Ename	Hookup
		Type	Local
		At	Head
		Part1	:Light_Flicker.part
	#ebee	Part2	:RedLightGlow.part
		Lifespan	3
	End
End

#############################################################


End