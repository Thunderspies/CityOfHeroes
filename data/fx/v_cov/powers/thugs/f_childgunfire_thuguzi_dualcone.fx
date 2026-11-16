#########################################################
##	forceBubble
########################################################
FxInfo



Flags    InheritAlpha

LifeSpan		300


######################################################################################################
########################################  Emitters (Left & right)  ############################################
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
###########################################   s h o t s  (left & right)   ############################################
######################################################################################################

Condition
	On 	Time
	Time 	29

	Event
		EName	Sounds
		Type	local
		At	Emitter_Right
		Sound uziburst4 80 80 .88
		lifespan 20
	End

	Event
		EName	GunFire_right
		Type	local
		At	Emitter_Right
	
		ChildFX	V_COV\Powers\Thugs\F_ChildGunFlare_ThugUzi.fx
		Lifespan 60
	End

	Event
		EName	GunFire_left
		Type	local
		At	Emitter_Left
	
		ChildFX	V_COV\Powers\Thugs\F_ChildGunFlare_ThugUzi.fx
		Lifespan 60
	End


End


######################################################################################################
###########################################   s h e l l s (left & right)   ############################################
######################################################################################################




Condition	
	On	Time
	Time	29

	Event
		EName	Shells_right
		Type	Local
		At	HandR
		bhvrOverride
			PositionOffset		.2 .1 .35
			PYRRotate		90 0 90
		End

		Childfx	:F_Shells_UziEject.fx
		lifespan	43
	End

	Event
		EName	Shells_left
		Type	Local
		At	HandL
		bhvrOverride
			PositionOffset		.2 .1 .35
			PYRRotate		90 0 90
		End

		Childfx	:F_Shells_UziEject.fx
		lifespan	43
	End


End
