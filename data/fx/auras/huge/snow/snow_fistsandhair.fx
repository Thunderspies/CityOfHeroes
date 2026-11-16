#############################################################
## Snow_FistsAndHair.fx
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

	Event
		Type	Local
		At	LArmL
		Part	:ColdMist_Additive_Local.part
	End

	Event
		Type	Local
		At	HandL
		Part	:ColdMist_Local.part
		Part	:ColdMist_Local_Trail.part
		Part	:Snowflakes.part
		Part	:ColdMist_Additive_Local.part
		Part	:ColdMist_Additive_Local.part
		Part	:ColdMist_Additive_Local_Trail.part
	End

	Event
		Type	Local
		At	LArmR
		Part	:ColdMist_Additive_Local.part
	End

	Event
		Type	Local
		At	HandR
		Part	:ColdMist_Local.part
		Part	:ColdMist_Local_Trail.part
		Part	:Snowflakes.part
		Part	:ColdMist_Additive_Local.part
		Part	:ColdMist_Additive_Local.part
		Part	:ColdMist_Additive_Local_Trail.part
	End
End

#########################################################

End