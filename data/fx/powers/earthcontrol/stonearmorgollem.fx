#########################################################
##	
##
FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	HIPS
End

Input  
	InpName	CHEST
End

Input  
	InpName	HEAD
End

Input  
	InpName	UARMR
End

Input  
	InpName	UARML
End

Input  
	InpName	LARML
End

Input  
	InpName	LARMR
End

Input  
	InpName	ULEGL
End

Input  
	InpName	ULEGR
End

Input  
	InpName	LLEGL
End

Input  
	InpName	LLEGR
End

Input  
	InpName	wepR
End

Input  
	InpName	wepL
End

Input  
	InpName	Root
End


##########################################################

##Stones Rolling

##########################################################


Condition
	On 	Time
	Time 	0

	Event
		EName 	StoneSetOne
		Type	local
		At	Root
		Anim	FX_stonearmor
	End


		
####################################################################
##Medium
####################################################################

	Event
		EName 	Stonesickle04
		Type	local
		At	StoneSetOne
		 AnimPiv Head
		Bhvr	behaviors/StonesRolling.bhvr
		geom	FX_Stone03
		
	End

	Event
		EName 	Stonesickle05
		Type	local
		At	StoneSetOne
		 AnimPiv Waist
		Bhvr	behaviors/StonesRolling.bhvr
		geom	FX_Stone02
		
	End

	Event
		EName 	Stonesickle06
		Type	local
		At	StoneSetOne
		 AnimPiv Neck
		Bhvr	behaviors/StonesRolling.bhvr
		geom	FX_Stone01
		
	End

	Event
		EName 	Stonesickle07
		Type	local
		At	StoneSetOne
		 AnimPiv Chest
		Bhvr	behaviors/StonesRolling.bhvr
		geom	FX_Stone03
		
	End

	Event
		EName 	Stonesickle08
		Type	local
		At	StoneSetOne
		 AnimPiv HandL
		Bhvr	behaviors/StonesRolling.bhvr
		geom	FX_Stone03
		
	End

	Event
		EName 	Stonesickle09
		Type	local
		At	StoneSetOne
		  AnimPiv Hair
		Bhvr	behaviors/StonesRolling.bhvr
		geom	FX_Stone02
		
	End
	
End


##########################################################
##Armor
##########################################################
Condition
	On 	Time
	Time 	20

	Event
		Type	Local 
		At	LLEGL
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	FX_StoneArmor_LLEGL
		

	End

	Event
		Type	Local 
		At	LLEGR
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	FX_StoneArmor_LLEGR
		
	End

End





###################################################################


Condition
	On 	Time
	Time 	25

	Event
		Type	Local 
		At	UlegR
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	FX_StoneArmor_ULEGL
	
	End

	Event
		Type	Local 
		At	ULEGL
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	FX_StoneArmor_ULEGR
	
	End
End


##########################################################

#Condition
#	On 	Time
#	Time 	30
#
#
##		Event
##		Type	Local 
##		At	Hips
##		Bhvr 	behaviors/StoneArmor.bhvr
##		geom	FX_StoneArmor_HIPS
##		
##	End
#
##	Event
##		Type	Local 
##		At	WepL
##		Bhvr 	behaviors/StoneArmor.bhvr
##		geom	FX_StoneArmor_WepL
##		
##	End
#
##	Event
##		Type	Local 
##		At	WepR
##		Bhvr 	behaviors/StoneArmor.bhvr
##		geom	FX_StoneArmor_WepR
##
##	End
#End
#
#################################################################


Condition
	On 	Time
	Time 	35


	Event
		Type	Local 
		At	CHEST
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	FX_StoneArmor_CHEST
	
	End

		Event
		Type	Local 
		At	LARMR
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	FX_StoneArmor_LARMR
		

	End

		Event
		Type	Local 
		At	LARML
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	FX_StoneArmor_LARML
		

	End

End

##############################################

Condition
	On 	Time
	Time 	40
	
	Event
		Type	Local 
		At	UARML
		Part1	:StoneArmorRocks.part
		#Part2	:StoneArmorRocks01.part
		#Part3	:StoneArmorRocks02.part
		#Part4	:StoneArmorRocks03.part
	End

End

Condition
	On 	Time
	Time 	40

	Event
		Type	Local 
		At	UARMR
		Part1	:StoneArmorRocks.part
		#Part2	:StoneArmorRocks01.part
		#Part3	:StoneArmorRocks02.part
		#Part4	:StoneArmorRocks03.part
	End

End

Condition
	On 	Time
	Time 	43

	Event
		Type	Local 
		At	UARML
		#Part1	:StoneArmorRocks.part
		Part2	:StoneArmorRocks01.part
		#Part3	:StoneArmorRocks02.part
		#Part4	:StoneArmorRocks03.part
	End

End

Condition
	On 	Time
	Time 	43

	Event
		Type	Local 
		At	UARMR
		#Part1	:StoneArmorRocks.part
		Part2	:StoneArmorRocks01.part
		#Part3	:StoneArmorRocks02.part
		#Part4	:StoneArmorRocks03.part
	End

End

Condition
	On 	Time
	Time 	47

	Event
		Type	Local 
		At	UARML
		#Part1	:StoneArmorRocks.part
		#Part2	:StoneArmorRocks01.part
		Part3	:StoneArmorRocks02.part
		#Part4	:StoneArmorRocks03.part
	End

End

Condition
	On 	Time
	Time 	47

	Event
		Type	Local 
		At	UARMR
		#Part1	:StoneArmorRocks.part
		#Part2	:StoneArmorRocks01.part
		Part3	:StoneArmorRocks02.part
		#Part4	:StoneArmorRocks03.part
	End

End

Condition
	On 	Time
	Time 	35

	Event
		Type	Local 
		At	UARML
		Part1	:StoneArmorDirt.part
		#Part2	:StoneArmorRocks01.part
		#Part3	:StoneArmorRocks02.part
		Part4	:StoneArmorRocks03.part
	End

	Event
		Type	Local 
		At	LARML
		Part1	:StoneArmorDirt.part
		#Part2	:StoneArmorRocks01.part
		#Part3	:StoneArmorRocks02.part
		#Part4	:StoneArmorRocks03.part
	End

End

Condition
	On 	Time
	Time 	37

	Event
		Type	Local 
		At	UARMR
		Part1	:StoneArmorDirt.part
		#Part2	:StoneArmorRocks01.part
		#Part3	:StoneArmorRocks02.part
		Part4	:StoneArmorRocks03.part
	End
	
	Event
		Type	Local 
		At	LARMR
		Part1	:StoneArmorDirt.part
		#Part2	:StoneArmorRocks01.part
		#Part3	:StoneArmorRocks02.part
		#Part4	:StoneArmorRocks03.part
	End
End


Condition
	On 	Time
	Time 	40

	Event
		Type	Local 
		At	UARML
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	FX_StoneArmor_UARML
		
	End

	Event
		Type	Local 
		At	UARMR
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	FX_StoneArmor_UARMR
	
	End
End


##########################################################



Condition
	On 	Time
	Time 	50

	Event
		Type	Local 
		At	Head
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	FX_StoneArmor_HEAD
				
	End
	
End


#################################################################
##Stone Kills
#################################################################

	
Condition

		On      Time
		Time	320

	Event	
		Ename	All
		Type	Destroy

	End


End


End	