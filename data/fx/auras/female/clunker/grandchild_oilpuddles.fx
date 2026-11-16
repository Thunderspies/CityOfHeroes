#############################################################
## Child_OilPuddles.fx
#############################################################

FxInfo

#############################################################

Condition
	On 		TRIGGERBITS #Cycle
	TRIGGERBITS    	FEMALE
	DoMany		1

	Event
		Ename	PuddleAnchor
		Type 	Local
		At	Origin
		Until	Fly
		Flags	OneAtATime
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 0.1 -0.5
		End
		ChildFX	:Child_OilPuddles.fx
	End
End

#############################################################
End