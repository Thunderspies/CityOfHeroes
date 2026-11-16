#########################################################
##	EBlast
########################################################
FxInfo

Input  
	Inpname	Chest
End

LifeSpan	190

#Condition
#	On Time 
#	Time 0
#
#	Repeat	6
#	
#	Event
#		EName	Sparkfar
#		Type 	local
#		At	Chest
#		
#		BhvrOverride
#			PositionOffset		0 3 0
#			FadeOutLength		280
#		End
#
#		ChildFX	V_COV\PhysicsEnabled\LightningHit.fx
#
#		LifeSpan	190
#
#	End
#
#End
#
#Condition
#	On Time 
#	Time 0
#
#	Repeat	5
#	
#
#	Event
#		EName	Sparkfar
#		Type 	local
#		At	Chest
#
#		BhvrOverride
#			PositionOffset		0 3 0
#			FadeOutLength		280
#		End
#		ChildFX	V_COV\PhysicsEnabled\ElectricSpark.fx
#
#		LifeSpan	190
#
#	End
#
#End
#

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

		ChildFX	Explosions/FireExplosion/LightningFireExplosionMedium_NoRing.fx

		LifeSpan	190

	End

End

End
