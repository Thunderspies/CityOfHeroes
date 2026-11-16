#########################################################
##	forceBubble
########################################################
FxInfo



Flags    InheritAlpha

LifeSpan		300


######################################################################################################
########################################  Emitters (Left)  ############################################
######################################################################################################

Condition
	On	Time
	Time	0


	Event	
		EName	Emitter_Left
		Type	Local
		At	HandL  
		BhvrOverride
			behavior
			PyrRotate		0 180 0
			positionOffset		-.8 -.094 .275
		End

	End
End





######################################################################################################
###########################################   s h o t s  (left)   ############################################
######################################################################################################

Condition
	On 	Time
	Time 	26

	Event
		EName	Sounds
		Type	local
		At	Emitter_Left
		Sound uziburst5 80 80 .88
		lifespan 5
	End


	Event
		EName	GunFire_left
		Type	local
		At	Emitter_Left
	
		ChildFX	V_COV\Powers\Thugs\F_ChildGunFlare_ThugUzi.fx
		Lifespan 15
	End


End


######################################################################################################
###########################################   s h e l l s (left)   ############################################
######################################################################################################




Condition	
	On	Time
	Time	29

	Event
		EName	Shells_left
		Type	Local
		At	WepL
		bhvrOverride
			PositionOffset		.2 .1 .35
			PYRRotate		90 0 90
		End

		Childfx	:F_Shells_UziEject.fx
		lifespan	12
	End


End
