#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	Root
End

##################################
Condition
	On	TriggerBits
	TriggerBits	concrete
	TriggerBits	grass		#needs grass clipping fx#
	TriggerBits	gravel		#needs gravel debris fx#
	TriggerBits	Dirt
	TriggerBits	Wood
	TriggerBits	Carpet
	TriggerBits	metal

	DoMany		1
		
	Event
		Ename	1
		Type	Local 
		At	FootR
		Until	Air
		Flags	OneAtaTime

		
		Part4	:LightSmoke.part
		
	End

	Event
		Ename	2
		Type	Local 
		At	FootL
		Until	Air
		Flags	OneAtaTime

		
		Part4	:LightSmoke.part
		
	End
	
	Event
		Ename	3
		Type	Local 
		At	LlegR
		Until	Air
		Flags	OneAtaTime

		
		Part4	:LightSmoke.part
		
	End
	
	Event
		Ename	4
		Type	Local 
		At	LlegL
		Until	Air
		Flags	OneAtaTime

		
		Part4	:LightSmoke.part
		
	End
	
	Event
		Ename	5
		Type	Local 
		At	UlegL
		Until	Air
		Flags	OneAtaTime

		
		Part4	:LightSmoke.part
		
	End
	
	Event
		Ename	6
		Type	Local 
		At	UlegR
		Until	Air
		Flags	OneAtaTime

		
		Part4	:LightSmoke.part
		
	End
	
	Event
		Ename	7
		Type	Local 
		At	Hips
		Until	Air
		Flags	OneAtaTime

		
		Part4	:LightSmoke.part
		
	End
	
	Event
		Ename	8
		Type	Local 
		At	back
		Until	Air
		Flags	OneAtaTime

		
		Part4	:LightSmoke.part
		
	End
	
	Event
		Ename	9
		Type	Local 
		At	Head
		Until	Air
		Flags	OneAtaTime

		
		Part4	:LightSmoke.part
		
	End
	
	Event
		Ename	10
		Type	Local 
		At	UArmL
		Until	Air
		Flags	OneAtaTime

		
		Part4	:LightSmoke.part
		
	End
	
	Event
		Ename	11
		Type	Local 
		At	UArmR
		Until	Air
		Flags	OneAtaTime

		
		Part4	:LightSmoke.part
		
	End
	
	Event
		Ename	12
		Type	Local 
		At	LArmR
		Until	Air
		Flags	OneAtaTime

		
		Part4	:LightSmoke.part
		
	End
	
	Event
		Ename	13
		Type	Local 
		At	LArmL
		Until	Air
		Flags	OneAtaTime

		
		Part4	:LightSmoke.part
		
	End
End

###################################	Forces	  ########################

Condition

	Event
		Ename	LevitationForce
		Type	Local
		At	Root
		
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		5
			PhysForcePower		60
			PhysForcePowerJitter	10
			PhysForceSpeedScaleMax	75
		End
	End

	
	Event
		Ename	WakeForce
		Type	Local
		At	Root
		
		BhvrOverride
			PhysForceType		In
			PhysForceRadius		4
			PhysForcePower		30
			PhysForcePowerJitter	5
			PhysForceSpeedScaleMax	75
		End
	End
	
End				