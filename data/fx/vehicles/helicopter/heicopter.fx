#########################################################
##	Hilcopter Rig
#########################################################

FxInfo

Flags InheritAlpha


############## sound  ###############################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Fuselage
		Type	Local
		At	Root
		BhvrOverride
			Scale 1 1 1
		End
		#Geom	_Helo01_L01__Helo1
		Sound HelicopterTurbine_Loop 150 150 .88
	End

###############  Main rotor   #############################################

	Event
		EName 	MainRotor	##(anim, minus geometry)
		Type	Local
		At	Fuselage
		BhvrOverride
			PositionOffset 0 26 0
			Scale		1 .8 1
		End
		Anim	HelicopterBlade
	End

	Event
		EName	MainRotor_node ##(refrence note from Anim)
		Type	local
		At	MainRotor  
		AnimPiv	Hips
		Sound HelicopterMainRotor_Loop 300 250 .88
	End

	Event
		EName 	MainRotor_geo  
		Type	Local
		At	MainRotor_node
		Geom	_Helo01_MainRotor_Blur
	End


############    Tail Rotor    #############################################



	Event
		EName 	TailRotor	##(anim, minus geometry)
		Type	Local
		At	Fuselage
		BhvrOverride
			PositionOffset 3 23.1 -55
			pyrRotate	0 0 90
			Scale		1 1 1
		End
		Anim	HelicopterBlade
	End

	Event
		EName	TailRotor_node ##(refrence note from Anim)
		Type	local
		At	TailRotor  
		AnimPiv	Hips
	End

	Event
		EName 	TailRotor_geo  
		Type	Local
		At	TailRotor_node
		Geom	_Helo01_TailRotor_Blur
		Sound HelicopterTailRotor_Loop 200 200 .88
	End

End	