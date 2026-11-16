#############################################################
## LampLight.fx
#############################################################

FxInfo
Lifespan 30

#############################################################

Condition
	On 	Cycle
	Time 	4
	Chance	0.75


	Event
		Ename	Hookup
		Type	Local
		At	Head
		Geom	FX_P_Spotlight
		Lifespan	2
	End
End

#############################################################


End