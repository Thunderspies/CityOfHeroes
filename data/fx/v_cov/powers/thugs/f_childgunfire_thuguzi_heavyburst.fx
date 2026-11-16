#########################################################
##	forceBubble
########################################################
FxInfo



Flags    InheritAlpha

LifeSpan		300








######################################################################################################
########################################  Emitters (Right)  ############################################
######################################################################################################

Condition
	On	Time
	Time	0


	Event	
		EName	Emitter_Right
		Type	Local
		At	HandR  
		BhvrOverride
			behavior
			positionOffset		.771 -.094 .275
		End

	End
End





######################################################################################################
###########################################   s h o t s  (Right)   ############################################
######################################################################################################

Condition
	On 	Time
	Time 	26

	Event
		EName	Sounds
		Type	local
		At	Emitter_Right
		Sound uziburst2 80 80 .88
		lifespan 5
	End


	Event
		EName	GunFire_Right
		Type	local
		At	Emitter_Right
	
		ChildFX	V_COV\Powers\Thugs\F_ChildGunFlare_ThugUzi.fx
		Lifespan 32
	End


End


######################################################################################################
###########################################   s h e l l s (Right)   ############################################
######################################################################################################




Condition	
	On	Time
	Time	29

	Event
		EName	Shells_Right
		Type	Local
		At	WepR
		bhvrOverride
			PositionOffset		.2 .1 .35
			PYRRotate		90 0 90
		End

		Childfx	:F_Shells_UziEject.fx
		lifespan	30
	End


End
