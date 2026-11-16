#############################################################
## MarkedForDeath_Continuing.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0


	Event
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0 2.5 0
		End
		Part	:Inspiration_General_Glow_Continuing.part
		Part	:Colin_Face.part
	End
End

#############################################################

End