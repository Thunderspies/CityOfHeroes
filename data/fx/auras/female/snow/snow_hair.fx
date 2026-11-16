#############################################################
## Snow_HeadAndEyes.fx
#############################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	PositOnly
		At	Hair
		Part	:ColdMist_Local.part
		Part	:ColdMist_Local_Trail.part
		Part	:Snowflakes.part
	End

	Event
		Type	Local
		At	Head
		Part	:ColdMist_Additive_Local.part
		Part	:ColdMist_Additive_Local.part
		Part	:ColdMist_Additive_Local_Trail.part
	End
End

#########################################################

End