#############################################################
## Child_OilPuddles.fx
#############################################################

FxInfo

#############################################################


Condition
	On 		TRIGGERBITS #Cycle
	TRIGGERBITS    	HUGE
	DoMany		1

	Event
		Ename	PuddleTrail
		Type 	Local
		At	Origin
		Until	Fly
		Flags	OneAtATime
		ChildFX	:Child_Puddle_Trail.fx
	End
End

#############################################################
End