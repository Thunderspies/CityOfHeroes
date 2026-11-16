#########################################################
##	Fire_Ball
##
FxInfo
##################################

#Condition
#	On	Time
#	Time	0
#	
#	Event
#		Type	Local
#		At	origin
#		
#		ChildFx	:FlightBoost.fx
#		
#
#	End
#
#End

Condition
	On	Time
	Time	10
	
	Event
		EName	2
		Type	Local
		At	FootR
		Part	:TendrilA.part
		Part	:FireProtectionBaseGlow.part
		part2	:IceArmorBits.part
		part3	:IceArmorBitsSmall.part
		part3	:Snowfall.part
		

	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		Part	:TendrilA.part
		Part	:FireProtectionBaseGlow.part
		part2	:IceArmorBits.part
		part3	:IceArmorBitsSmall.part
		part3	:Snowfall.part

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		Part	:TendrilA.part
		Part	:FireProtectionBaseGlow.part
		part2	:IceArmorBits.part
		part3	:IceArmorBitsSmall.part
		part3	:Snowfall.part
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		Part	:TendrilA.part
		Part	:FireProtectionBaseGlow.part
		part2	:IceArmorBits.part
		part3	:IceArmorBitsSmall.part
		part3	:Snowfall.part
	
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		Part	:TendrilA.part
		Part	:FireProtectionBaseGlow.part
		part2	:IceArmorBits.part
		part3	:IceArmorBitsSmall.part
		part3	:Snowfall.part

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		Part	:TendrilA.part
		Part	:FireProtectionBaseGlow.part
		part2	:IceArmorBits.part
		part3	:IceArmorBitsSmall.part
		part3	:Snowfall.part
	End

		
###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		Part	:TendrilA.part
		Part	:FireProtectionBaseGlow.part
		part2	:LiteIceArmorBits.part
		part3	:LiteIceArmorBitsSmall.part
		part3	:Snowfall.part
	End

		
##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		Part	:liteFireProtectionBaseGlow.part
		Part	:LiteTendrilA.part
		part2	:LiteIceArmorBits.part
		part3	:LiteIceArmorBitsSmall.part
		part3	:Snowfall.part

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		Part	:liteFireProtectionBaseGlow.part
		Part	:LiteTendrilA.part
		part2	:LiteIceArmorBits.part
		part3	:LiteIceArmorBitsSmall.part
		part3	:Snowfall.part
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		Part	:liteFireProtectionBaseGlow.part
		Part	:LiteTendrilA.part
		part2	:LiteIceArmorBits.part
		part3	:LiteIceArmorBitsSmall.part
		part3	:Snowfall.part

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		Part	:liteFireProtectionBaseGlow.part
		Part	:LiteTendrilA.part
		part2	:LiteIceArmorBits.part
		part3	:LiteIceArmorBitsSmall.part
		part3	:Snowfall.part
	End

End    


End	

