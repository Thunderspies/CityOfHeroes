#########################################################
##	EBlast
########################################################
FxInfo

Input  
	Inpname	Chest
End

LifeSpan	190

Condition
#	On Time 
#	Time 0

	Repeat	3
	
	Event
		EName	Sparkfar
		Type 	local
		At	Root
		
		BhvrOverride
			PositionOffset		0 1 0
			FadeOutLength		280
		End

		ChildFX	V_COV\PhysicsEnabled\RedLightningHit.fx

	End

	
	Event
		HardwareOnly
		EName	Sparkfar
		Type 	local
		At	Root
		
		BhvrOverride
			PositionOffset		0 1 0
			FadeOutLength		280
		End

		ChildFX	V_COV\PhysicsEnabled\RedElectricSpark.fx

	End

End

Condition
#	On Time 
#	Time 0

	Repeat	3
	

	Event
		EName	Sparkfar
		Type 	local
		At	root

		BhvrOverride
			PositionOffset		0 1 0
			FadeOutLength		280
		End
		ChildFX	V_COV\PhysicsEnabled\ElectricSpark.fx

	End

End

Condition
#	On Time 
#	Time 0

	Repeat	15
	

	Event
		HardwareOnly
		EName	Sparkfar
		Type 	local
		At	root

		BhvrOverride
			PositionOffset		0 1 0
			FadeOutLength		280
		End
		#ChildFX	V_COV\PhysicsEnabled\ElectricSpark.fx
		ChildFX	V_COV\PhysicsEnabled\RedElectricSpark.fx

	End

End

Condition
#	On Time 
#	Time 0
	

	Event
		EName	Sparkfar
		Type 	local
		At	Chest

		BhvrOverride
			FadeOutLength		280
		End

		ChildFX	Explosions/FireExplosion/FireExplosionMedium_NoRing2.fx

	End

End

End
