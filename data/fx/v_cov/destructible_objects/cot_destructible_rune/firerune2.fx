#########################################################
##	FireAura
FxInfo

##################################

Condition
	On	Time
	Time	0
	
#################################################################
##SOUND
#############################################################
	
#	Event
#		Type	local
#		At	root
#		Bhvr	powers/firecontrol/FireShield.bhvr
#		Sound	aura6 50 30 .24
#		Sound	aura5 50 30 .22
#		Sound	aura7 50 30 .19
#		LifeSpan	30
#	End

###########################################################################################
	
	Event
		EName	Pivot
		Type	Local
		At	Origin
		
		BhvrOverride	
			PositionOffset	0 4 0
		End

		part2	:FireCore.part

	End

	Event
		EName	Chest
		Type	Local
		At	Origin
		
		BhvrOverride	
			PositionOffset	0 4 0
		End

#		Part	:FireCoreUp.part
#		Part	:FireCoreUp2.part
		part3	V_COV/Powers/ThermalRadiation/FireCoreTrail2.part
		Part4	:FireCoreSparks2.part
	End
	
#############################################################

#	Event
#		EName	Pivot
#		Type	Local
#		At	Origin
#	
#		part2	:FireCore.part
#
#	End
#
#	Event
#		EName	Hips
#		Type	Local
#		At	Origin
#				
#		part3	V_COV/Powers/ThermalRadiation/FireCoreTrail1.part
#	End
#
#	Event
#		EName	Chest
#		Type	Local
#		At	Origin
#
#		
#		part2	:FireCore4.part
#	End
#
#	Event
#		EName	Chest
#		Type	Local
#		At	Origin
#		
#		part3	V_COV/Powers/ThermalRadiation/FireCoreTrail2.part
#		Part4	:FireCoreSparks.part
#	End
	
End		