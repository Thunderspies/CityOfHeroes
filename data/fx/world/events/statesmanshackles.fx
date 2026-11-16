#########################################################
##	Inferno FX
##
FxInfo
	

##########################################################################################################
##Final Blast
######################################################################################

Condition
	On	Time
	Time	0

	Event
		Type	posit
		At	WepR

		Part	WORLD\Events\HandEnergyDownRight.part
		Part	WORLD\Events\HandTendrilDownRight.part
		Part	WORLD\Events\HandEnergy.part
	End

	Event
		Type	posit
		At	WepL

		Part	WORLD\Events\HandEnergyDownLeft.part
		Part	WORLD\Events\HandTendrilDownLeft.part
		Part	WORLD\Events\HandEnergy.part
		

	End

	Event
		Type	posit
		At	FootR

		Part	WORLD\Events\HandEnergyUpRight.part
		Part	WORLD\Events\HandTendrilUpRight.part
		Part	WORLD\Events\FootEnergy.part

	End

	Event
		Type	posit
		At	FootL

		Part	WORLD\Events\HandEnergyUpLeft.part
		Part	WORLD\Events\HandTendrilUpLeft.part
		Part	WORLD\Events\FootEnergy.part
		
	End

End



End
