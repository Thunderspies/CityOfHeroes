#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	chest
End

Flags    InheritAlpha

########################################################
## hookup
########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	chest
		Geom	proplink
			
	End

	Event
		EName 	link1
		Type	Local 
		At	Prime
		Altpiv	1
		Sound heli_loop 50 50 .22
		Part1	:OscillatorSmoke.part
		Part2	:OscillatorSteam.part
		Part3	AnimatedCharacterParts/SprocketNeckSpark2.part
		Part4	AnimatedCharacterParts/SprocketNeckGlow.part
	End

	Event
		EName 	link2
		Type	Local 
		At	Prime
		 AltPiv 1
		Bhvr	behaviors/SprocketPropeller_spin.bhvr
		Geom	Propeller2
		
	End

End

Condition
	On 	triggerbits
	Triggerbits	death

	Event
		EName 	link1
		Type	Destroy
	
	End

	Event
		EName 	link2
		Type	Destroy
	
	End

	Event
		EName 	link3
		Type	Local 
		At	Prime
		 AltPiv 1
		
		Geom	Propeller

	End

End

End
##################################

