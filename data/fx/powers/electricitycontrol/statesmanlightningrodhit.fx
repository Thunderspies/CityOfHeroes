#########################################################
##	EBlast
########################################################
FxInfo

Input  
	Inpname	Chest
End

LifeSpan	190

Condition
	On Time 
	Time 0
	

	Event
		EName	Sparkfar
		Type 	local
		At	Chest

		BhvrOverride
			FadeOutLength		280
		End

		ChildFX	Explosions/FireExplosion/StatesmanLightningFireExplosion.fx

		LifeSpan	190

	End

End

End
