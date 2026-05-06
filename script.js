// Save data to localStorage
class ResumeManager {
    constructor() {
        this.loadFromStorage();
        this.setupEventListeners();
        this.setupWaveEffect();
        this.setupAutoSave();
    }

    // Load saved data from localStorage
    loadFromStorage() {
        const savedData = localStorage.getItem('resumeData');
        if (savedData) {
            const data = JSON.parse(savedData);
            for (const [field, value] of Object.entries(data)) {
                const element = document.querySelector(`[data-field="${field}"]`);
                if (element) {
                    element.textContent = value;
                }
            }
        }
    }

    // Save current data to localStorage
    saveToStorage() {
        const data = {};
        const editableElements = document.querySelectorAll('[contenteditable="true"]');
        
        editableElements.forEach(element => {
            const field = element.getAttribute('data-field');
            if (field) {
                data[field] = element.textContent;
            }
        });
        
        localStorage.setItem('resumeData', JSON.stringify(data));
        this.showNotification('Изменения сохранены');
    }

    // Reset to default data
    resetToDefault() {
        if (confirm('Вы уверены, что хотите сбросить все изменения?')) {
            localStorage.removeItem('resumeData');
            location.reload();
        }
    }

    // Show notification
    showNotification(message) {
        const notification = document.createElement('div');
        notification.className = 'save-notification';
        notification.textContent = message;
        document.body.appendChild(notification);
        
        setTimeout(() => {
            notification.remove();
        }, 2000);
    }

    // Add animation on edit
    addAnimationOnEdit(element) {
        element.addEventListener('input', () => {
            element.classList.add('highlight');
            setTimeout(() => {
                element.classList.remove('highlight');
            }, 500);
            this.saveToStorage();
        });
    }

    // Setup all event listeners
    setupEventListeners() {
        // Add animation to all editable elements
        const editableElements = document.querySelectorAll('[contenteditable="true"]');
        editableElements.forEach(element => {
            this.addAnimationOnEdit(element);
        });

        // Download PDF button
        const downloadBtn = document.getElementById('downloadBtn');
        if (downloadBtn) {
            downloadBtn.addEventListener('click', () => {
                this.downloadPDF();
            });
        }

        // Reset button
        const resetBtn = document.getElementById('resetBtn');
        if (resetBtn) {
            resetBtn.addEventListener('click', () => {
                this.resetToDefault();
            });
        }
    }

    // Download as PDF
    async downloadPDF() {
        const element = document.getElementById('resume');
        const downloadBtn = document.getElementById('downloadBtn');
        
        // Show loading state
        const originalText = downloadBtn.textContent;
        downloadBtn.textContent = '⏳ Создание PDF...';
        downloadBtn.disabled = true;
        
        try {
            const opt = {
                margin: [0.5, 0.5, 0.5, 0.5],
                filename: 'resume.pdf',
                image: { type: 'jpeg', quality: 0.98 },
                html2canvas: { scale: 2, useCORS: true },
                jsPDF: { unit: 'in', format: 'a4', orientation: 'portrait' }
            };
            
            await html2pdf().set(opt).from(element).save();
            this.showNotification('PDF успешно сохранен!');
        } catch (error) {
            console.error('PDF generation error:', error);
            this.showNotification('Ошибка при создании PDF');
        } finally {
            downloadBtn.textContent = originalText;
            downloadBtn.disabled = false;
        }
    }

    // Setup auto-save every 30 seconds
    setupAutoSave() {
        setInterval(() => {
            this.saveToStorage();
        }, 30000);
    }

    // Setup Material Wave Effect
    setupWaveEffect() {
        const waveElements = document.querySelectorAll('[data-ripple]');
        
        waveElements.forEach(element => {
            element.addEventListener('click', (e) => {
                this.createWave(e, element);
            });
        });
    }

    // Create wave animation
    createWave(event, element) {
        const rect = element.getBoundingClientRect();
        const x = event.clientX - rect.left;
        const y = event.clientY - rect.top;
        
        const wave = document.createElement('span');
        wave.className = 'wave';
        wave.style.left = `${x}px`;
        wave.style.top = `${y}px`;
        
        element.style.position = 'relative';
        element.style.overflow = 'hidden';
        element.appendChild(wave);
        
        setTimeout(() => {
            wave.remove();
        }, 600);
    }
}

// Initialize the application when DOM is loaded
document.addEventListener('DOMContentLoaded', () => {
    new ResumeManager();
});