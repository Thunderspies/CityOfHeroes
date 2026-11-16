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
		Ename	ShackleR
		Type	local
		At	handR

		Part	WORLD\Events\HandCuffsEnergyDownRight.part
		Part	WORLD\Events\HandCuffsDownRight.part
#		Part	WORLD\Events\HandEnergy.part
		
		Pmagnet	ShackleL
	End

	Event
		Ename	ShackleL
		Type	local
		At	handL

		Part	WORLD\Events\HandCuffsEnergyDownLeft.part
		Part	WORLD\Events\HandCuffsDownLeft.part
#		Part	WORLD\Events\HandEnergy.part
		
		Pmagnet	ShackleR

	End

End


End
