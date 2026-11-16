#############################################################
## Pixels_Body.fx
#############################################################

FxInfo

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	HandR
		Part	:Pixel_Sm.part
		Part	:Pixel_Run_Sm.part
		Part	:Pixel_Run_Sm.part
		#Part	:Pixel_Run2.part
	End

	Event
		Type	Local
		At	HandL
		Part	:Pixel_Sm.part
		Part	:Pixel_Run_Sm.part
		Part	:Pixel_Run_Sm.part
		#Part	:Pixel_Run2.part
	End


#######		GLOW

	Event
		Type	Local
		At	HandR
		part	:Pixel_Handglow.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		Type	Local
		At	HandR
		part	:Pixel_HandGlow.part
	End

	Event
		Type	Local
		At	HandL
		part	:Pixel_Handglow.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		Type	Local
		At	HandL
		part	:Pixel_HandGlow.part
	End
End
#########################################################
End