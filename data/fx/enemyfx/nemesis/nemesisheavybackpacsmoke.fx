#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	chest
End

Input  
	Inpname	LarmL
End


Flags    InheritAlpha
########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Chest
		Anim	FX_heavypilot
		Sound smoke2_loop 50 50 .5
	End

	Event
		EName 	Nemesis
		Type	local
		At	chest
		Geom	HeavyBackPac
		Sound nemesisbubble_loop 50 50 .6
	
	End

	Event
		EName 	dome
		Type	local
		At	chest
		Geom	NemesisFaceDome
	
	End
	
	Event
		EName 	NemesisSmoke1
		Type	local 
		At	Nemesis
		 AltPiv 1
		Part1	:Smoke.part
				
	End
	
	Event
		EName 	dome1
		Type	local 
		At	dome
		AltPiv 1
		Part1	:Bubbles.part
				
	End

	Event
		EName 	NemesisSmoke2
		Type	Local
		At	LarmL
		Anim	FX_heavyclaw
	End

End

Condition
	On 	triggerbits
	Triggerbits	death

	Event
		EName 	NemesisSmoke1
		Type	Destroy
	
	End

	Event
		EName 	NemesisSmoke2
		Type	Destroy
	
	End

	Event
		EName 	Dome
		Type	Destroy
	
	End

	Event
		EName 	Dome1
		Type	Destroy
	
	End
	
	Event
		EName 	Nemesis
		Type	Destroy
	
	End
	
	Event
		EName 	Prime
		Type	Destroy
	
	End
End
##################################

