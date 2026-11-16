#########################################################
##	
##

FxInfo
#LifeSpan  90

Input  
	InpName	Target
End

Input  
	InpName	Mystic
End

Input  
	InpName	WepR
End

##################################

Condition
	On Time
	Time 0

	Event
		Type	positonly
		At	Fore_ToeL 
		childfx	:bot_activate_Sweeper.fx
		lookat	Fore_FootL 
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	Fore_FootL  
		childfx	:bot_activate_Sweeper.fx
		lookat	Fore_LLegL 
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	Fore_LLegL  
		childfx	:bot_activate_Sweeper.fx
		lookat	ClawL 
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	ClawL  
		childfx	:bot_activate_Sweeper.fx
		lookat	Fore_ULegL 
		bhvroverride
			stretch 1
		end
	End
############## 
	Event
		Type	positonly
		At	HandL 
		childfx	:bot_activate_Sweeper.fx
		lookat	LarmL
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	LarmL 
		childfx	:bot_activate_Sweeper.fx
		lookat	UarmL
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	UarmL 
		childfx	:bot_activate_Sweeper.fx
		lookat	RingL
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	RingL 
		childfx	:bot_activate_Sweeper.fx
		lookat	Col_L
		bhvroverride
			stretch 1
		end
	End

##############
	Event
		Type	positonly
		At	Lwing4 
		childfx	:bot_activate_Sweeper.fx
		lookat	Lwing3 
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	Lwing3  
		childfx	:bot_activate_Sweeper.fx
		lookat	Lwing2 
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	Lwing2  
		childfx	:bot_activate_Sweeper.fx
		lookat	SpadL 
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	SpadL  
		childfx	:bot_activate_Sweeper.fx
		lookat	Lwing1 
		bhvroverride
			stretch 1
		end
	End
############## 
	Event
		Type	positonly
		At	ToeL 
		childfx	:bot_activate_Sweeper.fx
		lookat	FootL
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	FootL 
		childfx	:bot_activate_Sweeper.fx
		lookat	LlegL
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	LlegL 
		childfx	:bot_activate_Sweeper.fx
		lookat	BootL
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	BootL 
		childfx	:bot_activate_Sweeper.fx
		lookat	UlegL
		bhvroverride
			stretch 1
		end
	End


end
