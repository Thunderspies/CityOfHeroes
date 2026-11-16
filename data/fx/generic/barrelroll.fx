#########################################################
##	
##

FxInfo


Input  
	InpName	Mystic
End

Input  
	InpName	WepR
End

Input  
	InpName	Root
End

Input  
	InpName	head
End

##Lifespan	300

############# rolling barrel  #####################
Condition
	On 	Time
	Time 	0


	Event
		EName	Prop
		Type	Local
		At	Root
		Anim	FX_barrelroll
##		Bhvr	Behaviors/GenericParticleFade.bhvr

		End

End	
##########  kill roll barrel  ##########################

#Condition
#	On 	Death
#
#
#	Event
#		Ename 	Prop
#		Type	Destroy
#	End
#End


############ Drop lingering Barrel #######################

#Condition
#	On	Death
#
#	Event
#		EName	Drop_prop
#		Type	Start
#		At	Root
#		Geom	GEO_Large_barrel
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		Lifespan	300
#	End
#End

######### Explode FX ####################################



End
##################################